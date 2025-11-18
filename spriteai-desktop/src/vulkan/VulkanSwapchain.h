#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class VulkanSwapchain
{
public:
    VulkanSwapchain() = default;
    ~VulkanSwapchain() = default;

    // Şimdilik iskelet
    bool initialize() { return true; }
    void shutdown() {}

    // VulkanCanvasFramebuffer tarafından kullanılan getter’lar
    VkFormat getImageFormat() const { return m_imageFormat; }
    VkExtent2D getExtent()   const { return m_extent; }
    const std::vector<VkImageView>& getImageViews() const { return m_imageViews; }

private:
    VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
    VkFormat       m_imageFormat = VK_FORMAT_B8G8R8A8_UNORM;   // default
    VkExtent2D     m_extent{0, 0};
    std::vector<VkImageView> m_imageViews;
};
