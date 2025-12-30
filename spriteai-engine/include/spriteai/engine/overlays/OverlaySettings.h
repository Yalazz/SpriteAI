// spriteai-engine/include/spriteai/engine/overlays/OverlaySettings.h
#pragma once
#include <vector>

namespace spriteai::engine::overlays {

    struct GridSettings {
        bool enabled = true;
        float minorStep = 16.0f;
        float majorStep = 64.0f;
        float opacity = 0.35f;
    };

    enum class SymmetryAxis { Vertical, Horizontal };

    struct SymmetrySettings {
        bool enabled = false;
        SymmetryAxis axis = SymmetryAxis::Vertical;
        float axisX = 0.0f;
        float axisY = 0.0f;
        bool drawAxisLine = true;
    };

    struct GuideLine {
        bool vertical = true;
        float value = 0.0f;
        float opacity = 0.65f;
    };

    struct GuideSettings {
        bool enabled = true;
        std::vector<GuideLine> lines{};
    };

} // namespace spriteai::engine::overlays
