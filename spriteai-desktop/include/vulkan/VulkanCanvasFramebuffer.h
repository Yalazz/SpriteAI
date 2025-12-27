#pragma once

#include <vulkan/vulkan.h>
#include <QColor>
#include <vector>

class VulkanDevice;
class VulkanSwapchain;

class VulkanCanvasFramebuffer
{
public:
    VulkanCanvasFramebuffer(VulkanDevice* device, VulkanSwapchain* swapchain);
    ~VulkanCanvasFramebuffer();

    // Render pass + framebuffer üzerinden çizime başla / bitir
    void begin(VkCommandBuffer cmd, uint32_t imageIndex, const QColor& clearColor);
    void end(VkCommandBuffer cmd);

    VkRenderPass renderPass() const { return m_renderPass; }

private:
    VulkanDevice*      m_device;
    VulkanSwapchain*   m_swapchain;

    VkRenderPass                    m_renderPass = VK_NULL_HANDLE;
    std::vector<VkFramebuffer>      m_framebuffers;

    void createRenderPass();
    void createFramebuffers();
    void destroy();
};
