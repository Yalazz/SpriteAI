#include "VulkanEngine.h"

#include <vector>
#include <iostream>
#include <stdexcept>

VulkanEngine::VulkanEngine() = default;

VulkanEngine::~VulkanEngine()
{
    shutdown();
}

void VulkanEngine::initialize()
{
    if (m_initialized) {
        return;
    }

    createInstance();
    selectPhysicalDevice();
    createLogicalDevice();

    m_initialized = true;
    std::cout << "[SpriteAI::VulkanEngine] Vulkan başarıyla başlatıldı." << std::endl;
}

void VulkanEngine::shutdown()
{
    if (!m_initialized) {
        return;
    }

    if (m_device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(m_device);
        vkDestroyDevice(m_device, nullptr);
        m_device = VK_NULL_HANDLE;
    }

    if (m_instance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = VK_NULL_HANDLE;
    }

    m_physicalDevice = VK_NULL_HANDLE;
    m_initialized = false;
}

void VulkanEngine::createInstance()
{
    uint32_t apiVersion = 0;
    if (vkEnumerateInstanceVersion(&apiVersion) == VK_SUCCESS) {
        uint32_t major = VK_VERSION_MAJOR(apiVersion);
        uint32_t minor = VK_VERSION_MINOR(apiVersion);
        uint32_t patch = VK_VERSION_PATCH(apiVersion);
        std::cout << "[SpriteAI::VulkanEngine] Runtime Vulkan API version: "
                  << major << "." << minor << "." << patch << std::endl;
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "Sprite AI Desktop";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName        = "SpriteAIEngine";
    appInfo.engineVersion      = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion         = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

#ifdef __APPLE__
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("SpriteAI::VulkanEngine::createInstance: VkInstance oluşturulamadı.");
    }
}

void VulkanEngine::selectPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("SpriteAI::VulkanEngine::selectPhysicalDevice: Uygun Vulkan GPU bulunamadı.");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    for (auto device : devices) {
        if (isDeviceSuitable(device)) {
            m_physicalDevice = device;
            break;
        }
    }

    if (m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("SpriteAI::VulkanEngine::selectPhysicalDevice: Uygun GPU seçilemedi.");
    }
}

bool VulkanEngine::isDeviceSuitable(VkPhysicalDevice device) const
{
    VkPhysicalDeviceProperties props{};
    vkGetPhysicalDeviceProperties(device, &props);
    std::cout << "[SpriteAI::VulkanEngine] Kullanılan GPU: " << props.deviceName << std::endl;
    return true;
}

void VulkanEngine::createLogicalDevice()
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

    uint32_t graphicsFamilyIndex = UINT32_MAX;
    for (uint32_t i = 0; i < queueFamilyCount; ++i) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamilyIndex = i;
            break;
        }
    }

    if (graphicsFamilyIndex == UINT32_MAX) {
        throw std::runtime_error("SpriteAI::VulkanEngine::createLogicalDevice: Graphics queue ailesi bulunamadı.");
    }

    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = graphicsFamilyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

#ifdef __APPLE__
    const char* extensions[] = { "VK_KHR_portability_subset" };
    createInfo.enabledExtensionCount   = 1;
    createInfo.ppEnabledExtensionNames = extensions;
#endif

    VkResult result = vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("SpriteAI::VulkanEngine::createLogicalDevice: VkDevice oluşturulamadı.");
    }
}
