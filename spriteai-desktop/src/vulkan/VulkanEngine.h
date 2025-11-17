#pragma once

#include <vulkan/vulkan.h>

class VulkanEngine
{
public:
    VulkanEngine();
    ~VulkanEngine();

    void initialize();
    void shutdown();

    bool isInitialized() const { return m_initialized; }

private:
    bool             m_initialized    = false;
    VkInstance       m_instance       = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice         m_device         = VK_NULL_HANDLE;

    void createInstance();
    void selectPhysicalDevice();
    void createLogicalDevice();

    bool isDeviceSuitable(VkPhysicalDevice device) const;
};
