#pragma once

#include "ICanvasRenderer.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <QColor>
#include <QSize>
#include <QPointF>

class VulkanCanvasFramebuffer;
class VulkanContext;
class VulkanDevice;
class VulkanSwapchain;

class VulkanCanvasRenderer : public ICanvasRenderer
{
public:
    VulkanCanvasRenderer(
        VulkanContext* context,
        VulkanDevice* device,
        VulkanSwapchain* swapchain);

    ~VulkanCanvasRenderer();

    // ICanvasRenderer arayüzü
    void setCanvasSize(const QSize& size) override;
    void beginStroke(const QPointF& p, const QColor& color, float width) override;
    void continueStroke(const QPointF& p, float pressure = 1.0f) override;
    void endStroke() override;
    void clear(const QColor& color) override;

    // CPU tarafı için bir şey yapmıyoruz (Vulkan ile çizildiği için)
    void renderToPainter(QPainter&) override {}

    // Vulkan draw (Renderer içinden çağrılacak)
    void render(VkCommandBuffer cmd, uint32_t imageIndex);

private:
    VulkanContext*           m_context;
    VulkanDevice*            m_device;
    VulkanSwapchain*         m_swapchain;

    VulkanCanvasFramebuffer* m_framebuffer = nullptr;

    QColor m_background = Qt::white;

    struct StrokePoint { float x, y, pressure; };
    struct Stroke { QColor color; float width; std::vector<StrokePoint> pts; };

    std::vector<Stroke> m_strokes;
    Stroke*             m_current = nullptr;

    QSize               m_size;
};
