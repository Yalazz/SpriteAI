#include "../.h/VulkanCanvasRenderer.h"

#include "../vulkan/VulkanContext.h"
#include "../vulkan/VulkanDevice.h"
#include "../vulkan/VulkanSwapchain.h"
#include "../vulkan/VulkanCanvasFramebuffer.h"

VulkanCanvasRenderer::VulkanCanvasRenderer(
        VulkanContext* context,
        VulkanDevice* device,
        VulkanSwapchain* swapchain)
    : m_context(context)
    , m_device(device)
    , m_swapchain(swapchain)
{
    m_framebuffer = new VulkanCanvasFramebuffer(device, swapchain);
}

VulkanCanvasRenderer::~VulkanCanvasRenderer()
{
    delete m_framebuffer;
}

void VulkanCanvasRenderer::setCanvasSize(const QSize& size)
{
    m_size = size;
    // İleride: stroke koordinatlarını swapchain boyutuna scale edebilirsin.
}

void VulkanCanvasRenderer::beginStroke(const QPointF& p, const QColor& color, float width)
{
    m_strokes.emplace_back();
    m_current = &m_strokes.back();
    m_current->color = color;
    m_current->width = width;
    m_current->pts.push_back({ (float)p.x(), (float)p.y(), 1.0f });
}

void VulkanCanvasRenderer::continueStroke(const QPointF& p, float pressure)
{
    if (!m_current) return;
    m_current->pts.push_back({ (float)p.x(), (float)p.y(), pressure });
}

void VulkanCanvasRenderer::endStroke()
{
    m_current = nullptr;
}

void VulkanCanvasRenderer::clear(const QColor& color)
{
    m_background = color;
    m_strokes.clear();
    m_current = nullptr;
}

void VulkanCanvasRenderer::render(VkCommandBuffer cmd, uint32_t imageIndex)
{
    // 1) Render pass + clear
    m_framebuffer->begin(cmd, imageIndex, m_background);

    // 2) Şimdilik hiçbir şey çizme (pipeline yok).
    //    Sadece arkaplan clear olacak. Stroke’lar vektörlerde tutuluyor.
    //    Bir sonraki adımda burada GPU pipeline + vertex buffer ile stroke çizimi yapacağız.

    m_framebuffer->end(cmd);
}
