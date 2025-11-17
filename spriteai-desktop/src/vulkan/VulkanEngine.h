#pragma once

#include <vulkan/vulkan.h>

class VulkanEngine
{
public:
    VulkanEngine();
    ~VulkanEngine();

    // Vulkan’ı başlat / kapat
    void initialize();
    void shutdown();

    // Editor tarafında durum kontrolü için
    bool isInitialized() const { return m_initialized; }

private:
    bool             m_initialized    = false;
    VkInstance       m_instance       = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice         m_device         = VK_NULL_HANDLE;

    // İç fonksiyonlar
    void createInstance();
    void selectPhysicalDevice();
    void createLogicalDevice();

    bool isDeviceSuitable(VkPhysicalDevice device) const;
};
