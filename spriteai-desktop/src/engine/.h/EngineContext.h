#pragma once

#include <memory>
#include <string>

class VulkanContext;

class EngineContext
{
public:
    EngineContext();
    ~EngineContext();

    bool initialize();
    void shutdown();

    VulkanContext* vulkan() { return m_vulkanContext.get(); }
    const VulkanContext* vulkan() const { return m_vulkanContext.get(); }

private:
    bool initVulkan();

    std::unique_ptr<VulkanContext> m_vulkanContext;
};
