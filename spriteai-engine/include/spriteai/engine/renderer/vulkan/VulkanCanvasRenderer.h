#pragma once
#include <vector>
#include "spriteai/engine/renderer/ICanvasRenderer.h"

namespace spriteai::engine::renderer::vulkan {

// Vulkan backend is a stub in this skeleton.
// It keeps the same ICanvasRenderer API so editor can switch renderers later.
class VulkanCanvasRenderer final : public ICanvasRenderer {
public:
    void setCanvasSize(int w, int h) override;
    void clear(std::uint32_t rgba) override;
    void render(const spriteai::canvas::CanvasView& view) override;

    const std::uint32_t* rgba8Buffer() const override { return m_pixels.data(); }
    int width() const override { return m_w; }
    int height() const override { return m_h; }

private:
    int m_w = 0;
    int m_h = 0;
    std::vector<std::uint32_t> m_pixels;
};

} // namespace spriteai::engine::renderer::vulkan
