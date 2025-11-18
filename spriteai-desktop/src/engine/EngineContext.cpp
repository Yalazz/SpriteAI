#include "EngineContext.h"
#include "vulkan/VulkanContext.h"

#include <stdexcept>
#include <iostream>

EngineContext::EngineContext() = default;

EngineContext::~EngineContext()
{
    shutdown();
}

bool EngineContext::initialize()
{
    try {
        if (!initVulkan()) {
            std::cerr << "Vulkan initialization failed" << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Engine initialization exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool EngineContext::initVulkan()
{
    m_vulkanContext = std::make_unique<VulkanContext>();
    return m_vulkanContext->initialize();
}

void EngineContext::shutdown()
{
    if (m_vulkanContext) {
        m_vulkanContext->shutdown();
        m_vulkanContext.reset();
    }
}
