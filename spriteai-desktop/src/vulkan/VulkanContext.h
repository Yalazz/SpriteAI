#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include <string>

class VulkanContext
{
public:
    VulkanContext() = default;
    ~VulkanContext();

    bool initialize();
    void shutdown();

    VkInstance instance() const { return m_instance; }
    VkPhysicalDevice physicalDevice() const { return m_physicalDevice; }

private:
    bool createInstance();
    bool pickPhysicalDevice();

    std::vector<const char*> requiredInstanceExtensions() const;

private:
    VkInstance m_instance{VK_NULL_HANDLE};
    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
};
