#pragma once
#include <cstdint>
#include <vector>
#include "spriteai/engine/renderer/ICanvasRenderer.h"

namespace spriteai::engine::renderer::cpu {

class CpuCanvasRenderer final : public ICanvasRenderer {
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
    std::vector<std::uint32_t> m_pixels; // Qt RGBA8888 format

    void plot(int x, int y, std::uint32_t argb);
    void plotBlend(int x, int y, std::uint32_t argb);
    void drawLine(int x0, int y0, int x1, int y1, std::uint32_t argb);
    void drawThickLine(int x0, int y0, int x1, int y1, std::uint32_t argb, float width);
};

} // namespace spriteai::engine::renderer::cpu
