#include "spriteai/engine/renderer/cpu/CpuCanvasRenderer.h"
#include "spriteai/canvas/CanvasView.h"
#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/core/document/Layer.h"
#include "spriteai/engine/EngineContext.h"
#include "spriteai/core/selection/Selection.h"

#include <algorithm>
#include <cmath>

namespace spriteai::engine::renderer::cpu {

// Color format: ARGB (0xAARRGGBB) - used throughout the engine
static std::uint32_t argb_u8(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a=255) {
    return (std::uint32_t(a) << 24) | (std::uint32_t(r) << 16) | (std::uint32_t(g) << 8) | std::uint32_t(b);
}

// Convert ARGB to Qt Format_RGBA8888 (little-endian: 0xAABBGGRR)
static std::uint32_t argb_to_qt_rgba8888(std::uint32_t argb) {
    std::uint8_t a = (argb >> 24) & 0xFF;
    std::uint8_t r = (argb >> 16) & 0xFF;
    std::uint8_t g = (argb >> 8) & 0xFF;
    std::uint8_t b = argb & 0xFF;
    return (std::uint32_t(a) << 24) | (std::uint32_t(b) << 16) | (std::uint32_t(g) << 8) | std::uint32_t(r);
}

void CpuCanvasRenderer::setCanvasSize(int w, int h) {
    m_w = std::max(0, w);
    m_h = std::max(0, h);
    // Initialize with transparent black in Qt RGBA8888 format
    m_pixels.assign(std::size_t(m_w) * std::size_t(m_h), argb_to_qt_rgba8888(0xFF000000u));
}

void CpuCanvasRenderer::clear(std::uint32_t argb) {
    // Convert ARGB to Qt format for storage
    std::uint32_t qt_color = argb_to_qt_rgba8888(argb);
    std::fill(m_pixels.begin(), m_pixels.end(), qt_color);
}

void CpuCanvasRenderer::plot(int x, int y, std::uint32_t argb) {
    if (x < 0 || y < 0 || x >= m_w || y >= m_h) return;
    // Convert ARGB to Qt format for storage
    m_pixels[std::size_t(y) * std::size_t(m_w) + std::size_t(x)] = argb_to_qt_rgba8888(argb);
}

// Plot with alpha blending
void CpuCanvasRenderer::plotBlend(int x, int y, std::uint32_t argb) {
    if (x < 0 || y < 0 || x >= m_w || y >= m_h) return;

    std::uint8_t srcA = (argb >> 24) & 0xFF;
    if (srcA == 0) return; // Fully transparent, nothing to do
    if (srcA == 255) {
        // Fully opaque, just overwrite
        m_pixels[std::size_t(y) * std::size_t(m_w) + std::size_t(x)] = argb_to_qt_rgba8888(argb);
        return;
    }

    // Get existing pixel (in Qt format, convert back to ARGB for blending)
    std::uint32_t qt_dst = m_pixels[std::size_t(y) * std::size_t(m_w) + std::size_t(x)];
    std::uint8_t dstA = (qt_dst >> 24) & 0xFF;
    std::uint8_t dstB = (qt_dst >> 16) & 0xFF;
    std::uint8_t dstG = (qt_dst >> 8) & 0xFF;
    std::uint8_t dstR = qt_dst & 0xFF;

    std::uint8_t srcR = (argb >> 16) & 0xFF;
    std::uint8_t srcG = (argb >> 8) & 0xFF;
    std::uint8_t srcB = argb & 0xFF;

    // Standard alpha blending: out = src * srcA + dst * (1 - srcA)
    float sa = srcA / 255.0f;
    float da = 1.0f - sa;

    std::uint8_t outR = std::uint8_t(std::clamp(int(srcR * sa + dstR * da), 0, 255));
    std::uint8_t outG = std::uint8_t(std::clamp(int(srcG * sa + dstG * da), 0, 255));
    std::uint8_t outB = std::uint8_t(std::clamp(int(srcB * sa + dstB * da), 0, 255));
    std::uint8_t outA = std::uint8_t(std::clamp(int(srcA + dstA * da), 0, 255));

    // Store in Qt format
    m_pixels[std::size_t(y) * std::size_t(m_w) + std::size_t(x)] =
        (std::uint32_t(outA) << 24) | (std::uint32_t(outB) << 16) | (std::uint32_t(outG) << 8) | std::uint32_t(outR);
}

void CpuCanvasRenderer::drawLine(int x0, int y0, int x1, int y1, std::uint32_t argb) {
    // Bresenham
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    for (;;) {
        plotBlend(x0, y0, argb);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// Draw a thick line with given width
void CpuCanvasRenderer::drawThickLine(int x0, int y0, int x1, int y1, std::uint32_t argb, float width) {
    if (width <= 1.0f) {
        drawLine(x0, y0, x1, y1, argb);
        return;
    }

    int radius = static_cast<int>(width / 2.0f);

    // Draw circles at start and end for round caps
    auto drawCircle = [this, argb, radius](int cx, int cy) {
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                if (dx * dx + dy * dy <= radius * radius) {
                    plotBlend(cx + dx, cy + dy, argb);
                }
            }
        }
    };

    // Bresenham with thickness
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    for (;;) {
        drawCircle(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

static std::uint32_t to_u32(const spriteai::engine::ColorRGBA8& c, float alphaMul=1.0f) {
    auto a = std::uint8_t(std::clamp(int(float(c.a) * alphaMul), 0, 255));
    return argb_u8(c.r, c.g, c.b, a);
}

void CpuCanvasRenderer::render(const spriteai::canvas::CanvasView& view) {
    if (m_w <= 0 || m_h <= 0) return;

    auto* ctx = view.context();
    spriteai::engine::Theme theme{};
    if (ctx) theme = ctx->theme();

    // Clear to theme canvas color.
    clear(to_u32(theme.canvas.a ? theme.canvas : spriteai::engine::ColorRGBA8{12,18,25,255}));

    // Draw strokes from all visible layers (BEFORE grid overlay so grid is never erased)
    auto* doc = view.document();
    if (doc) {
        // Iterate through layers (bottom to top for proper compositing)
        for (const auto& layer : doc->layers()) {
            if (!layer.visible) continue;

            for (const auto& s : layer.strokes) {
                std::uint32_t col = s.rgba;

                // Apply layer opacity to stroke color
                if (layer.opacity < 1.0f) {
                    std::uint8_t alpha = (col >> 24) & 0xFF;
                    alpha = static_cast<std::uint8_t>(alpha * layer.opacity);
                    col = (static_cast<std::uint32_t>(alpha) << 24) | (col & 0x00FFFFFF);
                }

                if (col == 0x00000000u && ctx) {
                    col = to_u32(theme.canvas.a ? theme.canvas : spriteai::engine::ColorRGBA8{12,18,25,255});
                }

                if (s.isFill()) {
                    for (const auto& pt : s.points) {
                        float sx, sy;
                        view.worldToScreen(pt.x, pt.y, sx, sy);
                        plotBlend(int(sx), int(sy), col);
                    }
                } else if (s.isRectangle() && s.points.size() >= 2) {
                    float sx0, sy0, sx1, sy1;
                    view.worldToScreen(s.points[0].x, s.points[0].y, sx0, sy0);
                    view.worldToScreen(s.points[1].x, s.points[1].y, sx1, sy1);
                    int x0 = int(sx0), y0 = int(sy0);
                    int x1 = int(sx1), y1 = int(sy1);
                    if (x0 > x1) std::swap(x0, x1);
                    if (y0 > y1) std::swap(y0, y1);
                    for (int y = y0; y <= y1; ++y) {
                        for (int x = x0; x <= x1; ++x) {
                            plotBlend(x, y, col);
                        }
                    }
                } else if (!s.points.empty()) {
                    float prevSx, prevSy;
                    view.worldToScreen(s.points[0].x, s.points[0].y, prevSx, prevSy);

                    // Use stroke width for rendering, scale by zoom
                    float scaledWidth = s.width * view.camera().zoom;

                    if (scaledWidth <= 1.0f) {
                        plotBlend(int(prevSx), int(prevSy), col);
                    } else {
                        // Draw initial point as circle
                        int radius = static_cast<int>(scaledWidth / 2.0f);
                        for (int dy = -radius; dy <= radius; ++dy) {
                            for (int dx = -radius; dx <= radius; ++dx) {
                                if (dx * dx + dy * dy <= radius * radius) {
                                    plotBlend(int(prevSx) + dx, int(prevSy) + dy, col);
                                }
                            }
                        }
                    }

                    for (std::size_t i = 1; i < s.points.size(); ++i) {
                        float sx, sy;
                        view.worldToScreen(s.points[i].x, s.points[i].y, sx, sy);
                        drawThickLine(int(prevSx), int(prevSy), int(sx), int(sy), col, scaledWidth);
                        prevSx = sx;
                        prevSy = sy;
                    }
                }
            }
        }
    }

    // Grid overlay - only draw on unpainted (canvas-colored) pixels
    // This way: paint covers grid, eraser reveals grid
    if (ctx && ctx->grid().enabled) {
        const auto& grid = ctx->grid();
        auto minor = to_u32(theme.gridMinor.a ? theme.gridMinor : spriteai::engine::ColorRGBA8{30,42,56,255}, grid.opacity);
        auto major = to_u32(theme.gridMajor.a ? theme.gridMajor : spriteai::engine::ColorRGBA8{50,68,88,255}, std::min(1.0f, grid.opacity + 0.15f));

        // Canvas color in Qt format for comparison
        std::uint32_t canvasQt = argb_to_qt_rgba8888(to_u32(theme.canvas.a ? theme.canvas : spriteai::engine::ColorRGBA8{12,18,25,255}));

        // Compute world-space visible bounds
        float wx0, wy0, wx1, wy1;
        view.screenToWorld(0, 0, wx0, wy0);
        view.screenToWorld(float(m_w-1), float(m_h-1), wx1, wy1);
        if (wx0 > wx1) std::swap(wx0, wx1);
        if (wy0 > wy1) std::swap(wy0, wy1);

        // Helper to plot grid pixel only if current pixel is canvas color
        auto plotGridIfCanvas = [this, canvasQt](int x, int y, std::uint32_t argb) {
            if (x < 0 || y < 0 || x >= m_w || y >= m_h) return;
            std::size_t idx = std::size_t(y) * std::size_t(m_w) + std::size_t(x);
            if (m_pixels[idx] == canvasQt) {
                plotBlend(x, y, argb);
            }
        };

        // Draw grid line only on canvas-colored pixels
        auto drawGridLine = [&](int x0, int y0, int x1, int y1, std::uint32_t col) {
            int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
            int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
            int err = dx + dy;
            for (;;) {
                plotGridIfCanvas(x0, y0, col);
                if (x0 == x1 && y0 == y1) break;
                int e2 = 2 * err;
                if (e2 >= dy) { err += dy; x0 += sx; }
                if (e2 <= dx) { err += dx; y0 += sy; }
            }
        };

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
                drawGridLine(int(sx0), 0, int(sx1), m_h-1, col);
            }
            for (int y = startY; y <= endY; y += int(step)) {
                float sx0, sy0, sx1, sy1;
                view.worldToScreen(wx0, float(y), sx0, sy0);
                view.worldToScreen(wx1, float(y), sx1, sy1);
                drawGridLine(0, int(sy0), m_w-1, int(sy1), col);
            }
        };

        draw_grid_step(grid.minorStep, minor);
        draw_grid_step(grid.majorStep, major);
    }

    // Professional Symmetry Visualization
    if (ctx && ctx->symmetry().enabled && ctx->symmetry().drawAxisLine) {
        const auto& sym = ctx->symmetry();
        auto primaryCol = to_u32(theme.symmetry.a ? theme.symmetry : spriteai::engine::ColorRGBA8{255,127,219,255}, sym.lineOpacity);
        auto secondaryCol = to_u32(theme.symmetry.a ? theme.symmetry : spriteai::engine::ColorRGBA8{255,127,219,255}, sym.lineOpacity * 0.5f);
        auto centerCol = argb_u8(255, 255, 255, 220);

        // Get center point in screen coordinates
        float centerSx, centerSy;
        view.worldToScreen(sym.axisX, sym.axisY, centerSx, centerSy);
        int cx = int(centerSx);
        int cy = int(centerSy);

        // Helper lambda for dashed line
        auto drawDashedLine = [this](int x0, int y0, int x1, int y1, std::uint32_t col, int dashLen = 8, int gapLen = 4) {
            int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
            int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
            int err = dx + dy;
            int step = 0;
            int pattern = dashLen + gapLen;

            for (;;) {
                if (step % pattern < dashLen) {
                    plotBlend(x0, y0, col);
                }
                step++;
                if (x0 == x1 && y0 == y1) break;
                int e2 = 2 * err;
                if (e2 >= dy) { err += dy; x0 += sx; }
                if (e2 <= dx) { err += dx; y0 += sy; }
            }
        };

        // Draw based on symmetry type
        using SymType = spriteai::engine::overlays::SymmetryType;

        switch (sym.type) {
            case SymType::Vertical:
                drawDashedLine(cx, 0, cx, m_h-1, primaryCol, 10, 5);
                break;

            case SymType::Horizontal:
                drawDashedLine(0, cy, m_w-1, cy, primaryCol, 10, 5);
                break;

            case SymType::Both:
                drawDashedLine(cx, 0, cx, m_h-1, primaryCol, 10, 5);
                drawDashedLine(0, cy, m_w-1, cy, primaryCol, 10, 5);
                break;

            case SymType::Radial4:
            case SymType::Radial6:
            case SymType::Radial8: {
                int segments = (sym.type == SymType::Radial4) ? 4 :
                               (sym.type == SymType::Radial6) ? 6 : 8;

                float angleStep = 3.14159265f / segments;
                float radius = std::max(m_w, m_h) * 1.5f;

                for (int i = 0; i < segments; ++i) {
                    float angle = i * angleStep;
                    int x1 = cx + int(radius * std::cos(angle));
                    int y1 = cy + int(radius * std::sin(angle));
                    int x2 = cx - int(radius * std::cos(angle));
                    int y2 = cy - int(radius * std::sin(angle));
                    drawDashedLine(x1, y1, x2, y2, i == 0 ? primaryCol : secondaryCol, 8, 4);
                }
                break;
            }

            default:
                break;
        }

        // Draw center point indicator (professional style)
        if (sym.drawCenterPoint) {
            // Outer ring
            int ringRadius = 8;
            for (int dy = -ringRadius; dy <= ringRadius; ++dy) {
                for (int dx = -ringRadius; dx <= ringRadius; ++dx) {
                    int distSq = dx * dx + dy * dy;
                    if (distSq >= (ringRadius-1) * (ringRadius-1) && distSq <= ringRadius * ringRadius) {
                        plotBlend(cx + dx, cy + dy, primaryCol);
                    }
                }
            }

            // Inner crosshair
            for (int i = -4; i <= 4; ++i) {
                if (i != 0) {
                    plotBlend(cx + i, cy, centerCol);
                    plotBlend(cx, cy + i, centerCol);
                }
            }

            // Center dot
            plotBlend(cx, cy, centerCol);
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

    // Selection overlay (marching ants style)
    if (ctx) {
        const auto& sel = ctx->selection();
        if (sel.hasSelection()) {
            const auto& bounds = sel.bounds();

            float sx0, sy0, sx1, sy1;
            view.worldToScreen(bounds.x, bounds.y, sx0, sy0);
            view.worldToScreen(bounds.x + bounds.width, bounds.y + bounds.height, sx1, sy1);

            int x0 = int(sx0), y0 = int(sy0);
            int x1 = int(sx1), y1 = int(sy1);
            if (x0 > x1) std::swap(x0, x1);
            if (y0 > y1) std::swap(y0, y1);

            std::uint32_t selColor = argb_u8(100, 150, 255, 200);

            // Draw selection rectangle outline (dashed pattern)
            for (int x = x0; x <= x1; ++x) {
                if ((x + y0) % 4 < 2) plot(x, y0, selColor);
                if ((x + y1) % 4 < 2) plot(x, y1, selColor);
            }
            for (int y = y0; y <= y1; ++y) {
                if ((x0 + y) % 4 < 2) plot(x0, y, selColor);
                if ((x1 + y) % 4 < 2) plot(x1, y, selColor);
            }
        }
    }

}

} // namespace spriteai::engine::renderer::cpu
