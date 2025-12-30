#include "spriteai/engine/renderer/cpu/CpuCanvasRenderer.h"
#include "spriteai/canvas/CanvasView.h"
#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/engine/EngineContext.h"

#include <algorithm>
#include <cmath>

namespace spriteai::engine::renderer::cpu {

static std::uint32_t rgba_u8(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a=255) {
    return (std::uint32_t(r) << 24) | (std::uint32_t(g) << 16) | (std::uint32_t(b) << 8) | std::uint32_t(a);
}

void CpuCanvasRenderer::setCanvasSize(int w, int h) {
    m_w = std::max(0, w);
    m_h = std::max(0, h);
    m_pixels.assign(std::size_t(m_w) * std::size_t(m_h), 0x000000FFu);
}

void CpuCanvasRenderer::clear(std::uint32_t rgba) {
    std::fill(m_pixels.begin(), m_pixels.end(), rgba);
}

void CpuCanvasRenderer::plot(int x, int y, std::uint32_t rgba) {
    if (x < 0 || y < 0 || x >= m_w || y >= m_h) return;
    m_pixels[std::size_t(y) * std::size_t(m_w) + std::size_t(x)] = rgba;
}

void CpuCanvasRenderer::drawLine(int x0, int y0, int x1, int y1, std::uint32_t rgba) {
    // Bresenham
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    for (;;) {
        plot(x0, y0, rgba);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

static std::uint32_t to_u32(const spriteai::engine::ColorRGBA8& c, float alphaMul=1.0f) {
    auto a = std::uint8_t(std::clamp(int(float(c.a) * alphaMul), 0, 255));
    return rgba_u8(c.r, c.g, c.b, a);
}

void CpuCanvasRenderer::render(const spriteai::canvas::CanvasView& view) {
    if (m_w <= 0 || m_h <= 0) return;

    auto* ctx = view.context();
    spriteai::engine::Theme theme{};
    if (ctx) theme = ctx->theme();

    // Clear to theme canvas color.
    clear(to_u32(theme.canvas.a ? theme.canvas : spriteai::engine::ColorRGBA8{12,18,25,255}));

    // Grid overlay (engine-side)
    if (ctx && ctx->grid().enabled) {
        const auto& grid = ctx->grid();
        auto minor = to_u32(theme.gridMinor.a ? theme.gridMinor : spriteai::engine::ColorRGBA8{30,42,56,255}, grid.opacity);
        auto major = to_u32(theme.gridMajor.a ? theme.gridMajor : spriteai::engine::ColorRGBA8{50,68,88,255}, std::min(1.0f, grid.opacity + 0.15f));

        // Compute world-space visible bounds
        float wx0, wy0, wx1, wy1;
        view.screenToWorld(0, 0, wx0, wy0);
        view.screenToWorld(float(m_w-1), float(m_h-1), wx1, wy1);
        if (wx0 > wx1) std::swap(wx0, wx1);
        if (wy0 > wy1) std::swap(wy0, wy1);

        auto draw_grid_step = [&](float step, std::uint32_t col) {
            if (step <= 1e-3f) return;
            int startX = int(std::floor(wx0 / step) * step);
            int endX   = int(std::ceil(wx1 / step) * step);
            int startY = int(std::floor(wy0 / step) * step);
            int endY   = int(std::ceil(wy1 / step) * step);

            for (int x = startX; x <= endX; x += int(step)) {
                float sx0, sy0, sx1, sy1;
                view.worldToScreen(float(x), wy0, sx0, sy0);
                view.worldToScreen(float(x), wy1, sx1, sy1);
                drawLine(int(sx0), 0, int(sx1), m_h-1, col);
            }
            for (int y = startY; y <= endY; y += int(step)) {
                float sx0, sy0, sx1, sy1;
                view.worldToScreen(wx0, float(y), sx0, sy0);
                view.worldToScreen(wx1, float(y), sx1, sy1);
                drawLine(0, int(sy0), m_w-1, int(sy1), col);
            }
        };

        draw_grid_step(grid.minorStep, minor);
        draw_grid_step(grid.majorStep, major);
    }

    // Draw strokes
    auto* doc = view.document();
    if (doc) {
        for (const auto& s : doc->strokes()) {
            std::uint32_t col = s.rgba;
            if (col == 0x00000000u && ctx) {
                // "eraser": paint with canvas color (simple)
                col = to_u32(theme.canvas.a ? theme.canvas : spriteai::engine::ColorRGBA8{12,18,25,255});
            }
            for (const auto& pt : s.points) {
                float sx, sy;
                view.worldToScreen(pt.x, pt.y, sx, sy);
                plot(int(sx), int(sy), col);
            }
        }
    }

    // Symmetry axis & mirrored preview (render-only)
    if (ctx && ctx->symmetry().enabled && ctx->symmetry().drawAxisLine) {
        const auto& sym = ctx->symmetry();
        auto col = to_u32(theme.symmetry.a ? theme.symmetry : spriteai::engine::ColorRGBA8{255,127,219,255}, 0.85f);
        if (sym.axis == spriteai::engine::overlays::SymmetryAxis::Vertical) {
            float sx, sy;
            view.worldToScreen(sym.axisX, 0.0f, sx, sy);
            drawLine(int(sx), 0, int(sx), m_h-1, col);
        } else {
            float sx, sy;
            view.worldToScreen(0.0f, sym.axisY, sx, sy);
            drawLine(0, int(sy), m_w-1, int(sy), col);
        }
    }

    // Guide lines
    if (ctx && ctx->guides().enabled) {
        auto col = to_u32(theme.guide.a ? theme.guide : spriteai::engine::ColorRGBA8{127,209,255,255}, 0.75f);
        for (const auto& g : ctx->guides().lines) {
            if (g.vertical) {
                float sx, sy;
                view.worldToScreen(g.value, 0.0f, sx, sy);
                drawLine(int(sx), 0, int(sx), m_h-1, col);
            } else {
                float sx, sy;
                view.worldToScreen(0.0f, g.value, sx, sy);
                drawLine(0, int(sy), m_w-1, int(sy), col);
            }
        }
    }

}

} // namespace spriteai::engine::renderer::cpu
