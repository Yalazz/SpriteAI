#pragma once
#include <cstdint>
#include <functional>
#include <optional>
#include "spriteai/engine/overlays/OverlaySettings.h"

namespace spriteai::core::selection { class Selection; }
namespace spriteai::core::document { class SpriteDocument; }

namespace spriteai::core::tools {

struct PixelSampler {
    std::function<std::optional<std::uint32_t>(float worldX, float worldY)> sampleAt;
    int bufferWidth = 0;
    int bufferHeight = 0;
};

struct ColorChangedCallback {
    std::function<void(std::uint32_t rgba)> onColorPicked;
};

class ToolContext {
public:
    ToolContext() = default;

    void setSelection(selection::Selection* sel) { m_selection = sel; }
    selection::Selection* selection() const { return m_selection; }

    void setPixelSampler(const PixelSampler& sampler) { m_sampler = sampler; }
    const PixelSampler& pixelSampler() const { return m_sampler; }

    void setColorCallback(const ColorChangedCallback& cb) { m_colorCallback = cb; }
    const ColorChangedCallback& colorCallback() const { return m_colorCallback; }

    void setForegroundColor(std::uint32_t rgba) { m_fgColor = rgba; }
    std::uint32_t foregroundColor() const { return m_fgColor; }

    void setBackgroundColor(std::uint32_t rgba) { m_bgColor = rgba; }
    std::uint32_t backgroundColor() const { return m_bgColor; }

    void setSymmetry(spriteai::engine::overlays::SymmetrySettings* sym) { m_symmetry = sym; }
    spriteai::engine::overlays::SymmetrySettings* symmetry() const { return m_symmetry; }

private:
    selection::Selection* m_selection = nullptr;
    PixelSampler m_sampler{};
    ColorChangedCallback m_colorCallback{};
    std::uint32_t m_fgColor = 0xFFFFFFFF;
    std::uint32_t m_bgColor = 0x000000FF;
    spriteai::engine::overlays::SymmetrySettings* m_symmetry = nullptr;
};

} // namespace spriteai::core::tools
