#include "VulkanCanvasFramebuffer.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

#include <stdexcept>
#include <array>


VulkanCanvasFramebuffer::VulkanCanvasFramebuffer(VulkanDevice* device,
                                                 VulkanSwapchain* swapchain)
    : m_device(device)
    , m_swapchain(swapchain)
{
    createRenderPass();
    createFramebuffers();
}

VulkanCanvasFramebuffer::~VulkanCanvasFramebuffer()
{
    destroy();
}

void VulkanCanvasFramebuffer::destroy()
{
    if (!m_device) return;

    VkDevice device = m_device->getDevice();

    for (auto fb : m_framebuffers)
    {
        if (fb != VK_NULL_HANDLE)
            vkDestroyFramebuffer(device, fb, nullptr);
    }
    m_framebuffers.clear();

    if (m_renderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(device, m_renderPass, nullptr);
        m_renderPass = VK_NULL_HANDLE;
    }
}

void VulkanCanvasFramebuffer::createRenderPass()
{
    VkDevice device = m_device->getDevice();
    VkFormat swapchainFormat = m_swapchain->getImageFormat();

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format         = swapchainFormat;
    colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorRef{};
    colorRef.attachment = 0;
    colorRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &colorRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass    = 0;
    dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo rpInfo{};
    rpInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpInfo.attachmentCount = 1;
    rpInfo.pAttachments    = &colorAttachment;
    rpInfo.subpassCount    = 1;
    rpInfo.pSubpasses      = &subpass;
    rpInfo.dependencyCount = 1;
    rpInfo.pDependencies   = &dependency;

    if (vkCreateRenderPass(device, &rpInfo, nullptr, &m_renderPass) != VK_SUCCESS)
        throw std::runtime_error("Failed to create VulkanCanvasFramebuffer render pass");
}

void VulkanCanvasFramebuffer::createFramebuffers()
{
    VkDevice device = m_device->getDevice();
    VkExtent2D extent = m_swapchain->getExtent();
    const auto& views = m_swapchain->getImageViews();

    m_framebuffers.resize(views.size());

    for (size_t i = 0; i < views.size(); ++i)
    {
        VkImageView attachments[] = { views[i] };

        VkFramebufferCreateInfo fbInfo{};
        fbInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbInfo.renderPass      = m_renderPass;
        fbInfo.attachmentCount = 1;
        fbInfo.pAttachments    = attachments;
        fbInfo.width           = extent.width;
        fbInfo.height          = extent.height;
        fbInfo.layers          = 1;

        if (vkCreateFramebuffer(device, &fbInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS)
            throw std::runtime_error("Failed to create VulkanCanvasFramebuffer framebuffer");
    }
}

void VulkanCanvasFramebuffer::begin(VkCommandBuffer cmd,
                                    uint32_t imageIndex,
                                    const QColor& clearColor)
{
    if (imageIndex >= m_framebuffers.size())
        return;

    VkClearValue clear{};
    clear.color.float32[0] = clearColor.redF();
    clear.color.float32[1] = clearColor.greenF();
    clear.color.float32[2] = clearColor.blueF();
    clear.color.float32[3] = clearColor.alphaF();

    VkExtent2D extent = m_swapchain->getExtent();

    VkRenderPassBeginInfo beginInfo{};
    beginInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.renderPass        = m_renderPass;
    beginInfo.framebuffer       = m_framebuffers[imageIndex];
    beginInfo.renderArea.offset = {0, 0};
    beginInfo.renderArea.extent = extent;
    beginInfo.clearValueCount   = 1;
    beginInfo.pClearValues      = &clear;

    vkCmdBeginRenderPass(cmd, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCanvasFramebuffer::end(VkCommandBuffer cmd)
{
    vkCmdEndRenderPass(cmd);
}
