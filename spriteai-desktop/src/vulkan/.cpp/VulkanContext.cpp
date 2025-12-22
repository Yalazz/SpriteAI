#include "../.h/VulkanContext.h"

#include <iostream>
#include <stdexcept>
#include <cstring>

VulkanContext::~VulkanContext()
{
    shutdown();
}

bool VulkanContext::initialize()
{
    if (!createInstance()) {
        std::cerr << "Failed to create Vulkan instance" << std::endl;
        return false;
    }

    if (!pickPhysicalDevice()) {
        std::cerr << "Failed to pick Vulkan physical device" << std::endl;
        return false;
    }

    return true;
}

void VulkanContext::shutdown()
{
    if (m_instance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = VK_NULL_HANDLE;
    }
}
std::vector<const char*> VulkanContext::requiredInstanceExtensions() const
{
    std::vector<const char*> extensions;
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#ifdef __APPLE__
    // En garantili (her SDK’da çalışır)
    extensions.push_back("VK_EXT_metal_surface");

    // Eğer makro varsa çalışır
    // extensions.push_back(VK_MVK_METAL_SURFACE_EXTENSION_NAME);

    // Portability layer
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

    return extensions;
}


bool VulkanContext::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Sprite AI Desktop";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "SpriteAIEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    auto extensions = requiredInstanceExtensions();

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef __APPLE__
    // macOS için zorunlu
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
    if (result != VK_SUCCESS) {
        std::cerr << "vkCreateInstance failed with error code " << result << std::endl;
        return false;
    }

    return true;
}

bool VulkanContext::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    VkResult result = vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (result != VK_SUCCESS || deviceCount == 0) {
        std::cerr << "vkEnumeratePhysicalDevices failed or returned zero devices" << std::endl;
        return false;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    result = vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
    if (result != VK_SUCCESS) {
        std::cerr << "vkEnumeratePhysicalDevices (2) failed" << std::endl;
        return false;
    }

    m_physicalDevice = devices[0];
    return true;
}
