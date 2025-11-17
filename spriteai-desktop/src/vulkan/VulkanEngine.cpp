#include "VulkanEngine.h"

#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstring>

// -----------------------------------------------------------------------------
//  Extension isimleri tüm SDK sürümleriyle uyumlu olsun diye fallback ekliyoruz
// -----------------------------------------------------------------------------
#ifndef VK_MVK_MACOS_SURFACE_EXTENSION_NAME
#define VK_MVK_MACOS_SURFACE_EXTENSION_NAME "VK_MVK_macos_surface"
#endif

#ifndef VK_EXT_METAL_SURFACE_EXTENSION_NAME
#define VK_EXT_METAL_SURFACE_EXTENSION_NAME "VK_EXT_metal_surface"
#endif

#ifndef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
#define VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME "VK_KHR_portability_subset"
#endif
// -----------------------------------------------------------------------------

VulkanEngine::VulkanEngine() = default;

VulkanEngine::~VulkanEngine()
{
    shutdown();
}

void VulkanEngine::initialize()
{
    if (m_initialized)
        return;

    createInstance();
    selectPhysicalDevice();
    createLogicalDevice();

    m_initialized = true;
    std::cout << "[SpriteAI::VulkanEngine] Vulkan başarıyla başlatıldı." << std::endl;
}

void VulkanEngine::shutdown()
{
    if (!m_initialized)
        return;

    if (m_device != VK_NULL_HANDLE)
    {
        vkDeviceWaitIdle(m_device);
        vkDestroyDevice(m_device, nullptr);
        m_device = VK_NULL_HANDLE;
    }

    if (m_instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = VK_NULL_HANDLE;
    }

    m_physicalDevice = VK_NULL_HANDLE;
    m_initialized = false;
}

void VulkanEngine::createInstance()
{
    uint32_t apiVersion = 0;
    vkEnumerateInstanceVersion(&apiVersion);

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "SpriteAI Desktop";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "SpriteAIEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    // -------------------------
    // macOS için zorunlu extensions
    // -------------------------
    std::vector<const char*> extensions = {
        "VK_KHR_surface",
        "VK_EXT_metal_surface",
        "VK_KHR_get_physical_device_properties2",
        "VK_KHR_portability_enumeration",
        "VK_KHR_get_surface_capabilities2",
        "VK_EXT_swapchain_colorspace",
        "VK_KHR_surface_protected_capabilities"
    };

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

#if defined(__APPLE__)
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    createInfo.enabledExtensionCount = (uint32_t)extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Vulkan Instance oluşturulamadı.");
    }
}


void VulkanEngine::selectPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (deviceCount == 0)
        throw std::runtime_error("Uygun Vulkan GPU bulunamadı.");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    for (auto d : devices)
    {
        if (isDeviceSuitable(d))
        {
            m_physicalDevice = d;
            break;
        }
    }

    if (m_physicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("Uygun GPU seçilemedi.");
}

bool VulkanEngine::isDeviceSuitable(VkPhysicalDevice device) const
{
    VkPhysicalDeviceProperties props{};
    vkGetPhysicalDeviceProperties(device, &props);

    std::cout << "[SpriteAI::VulkanEngine] Kullanılan GPU: "
              << props.deviceName << std::endl;

    return true;
}
void VulkanEngine::createLogicalDevice()
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

    uint32_t graphicsFamilyIndex = 0;
    for (uint32_t i = 0; i < queueFamilyCount; ++i) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamilyIndex = i;
            break;
        }
    }

    float priority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueCount = 1;
    queueInfo.queueFamilyIndex = graphicsFamilyIndex;
    queueInfo.pQueuePriorities = &priority;

    std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#if defined(__APPLE__)
        VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
#endif
    };

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueInfo;
    createInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("Logical Device oluşturulamadı.");
    }
}
