#include "spriteai/engine/renderer/vulkan/VulkanCanvasRenderer.h"
#include <algorithm>

namespace spriteai::engine::renderer::vulkan {

void VulkanCanvasRenderer::setCanvasSize(int w, int h) {
    m_w = std::max(0, w);
    m_h = std::max(0, h);
    m_pixels.assign(std::size_t(m_w) * std::size_t(m_h), 0x000000FFu);
}

void VulkanCanvasRenderer::clear(std::uint32_t rgba) {
    std::fill(m_pixels.begin(), m_pixels.end(), rgba);
}

void VulkanCanvasRenderer::render(const spriteai::canvas::CanvasView&) {
    // Stub: nothing yet
}

} // namespace spriteai::engine::renderer::vulkan
