#pragma once

#include <vulkan/vulkan.h>

class VulkanDevice
{
public:
    VulkanDevice() = default;
    ~VulkanDevice() = default;

    // Şimdilik sadece iskelet – ileride gerçek init yazacağız
    bool initialize() { return true; }
    void shutdown() {}

    // Canvas / framebuffer için gereken getter’lar
    VkDevice getDevice() const { return m_device; }

    VkQueue getGraphicsQueue() const { return m_graphicsQueue; }
    uint32_t getGraphicsQueueFamilyIndex() const { return m_graphicsQueueFamilyIndex; }

private:
    VkDevice  m_device = VK_NULL_HANDLE;
    VkQueue   m_graphicsQueue = VK_NULL_HANDLE;
    uint32_t  m_graphicsQueueFamilyIndex = 0;
};
