#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <QColor>

class VulkanDevice;

class VulkanCanvasPipeline
{
public:
    VulkanCanvasPipeline(VulkanDevice* device);
    ~VulkanCanvasPipeline();

    void bind(VkCommandBuffer cmd);

    struct Stroke {
        QColor color;
        float width;
        std::vector<float> vertices;
    };

    void drawStroke(VkCommandBuffer cmd, const Stroke& s);

private:
    VulkanDevice* m_device;
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout m_layout = VK_NULL_HANDLE;

    void createPipeline();
};
