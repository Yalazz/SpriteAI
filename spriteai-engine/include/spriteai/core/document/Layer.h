#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "spriteai/core/document/Stroke.h"

namespace spriteai::core::document {

enum class BlendMode : std::uint8_t {
    Normal = 0,
    Multiply,
    Screen,
    Overlay,
    Darken,
    Lighten,
    ColorDodge,
    ColorBurn,
    HardLight,
    SoftLight,
    Difference,
    Exclusion
};

struct Layer {
    std::string name = "Layer";
    std::vector<Stroke> strokes;
    float opacity = 1.0f;
    BlendMode blendMode = BlendMode::Normal;
    bool visible = true;
    bool locked = false;

    Layer() = default;
    explicit Layer(const std::string& n) : name(n) {}
};

// Convert blend mode to string
inline const char* blendModeToString(BlendMode mode) {
    switch (mode) {
        case BlendMode::Normal: return "Normal";
        case BlendMode::Multiply: return "Multiply";
        case BlendMode::Screen: return "Screen";
        case BlendMode::Overlay: return "Overlay";
        case BlendMode::Darken: return "Darken";
        case BlendMode::Lighten: return "Lighten";
        case BlendMode::ColorDodge: return "Color Dodge";
        case BlendMode::ColorBurn: return "Color Burn";
        case BlendMode::HardLight: return "Hard Light";
        case BlendMode::SoftLight: return "Soft Light";
        case BlendMode::Difference: return "Difference";
        case BlendMode::Exclusion: return "Exclusion";
        default: return "Normal";
    }
}

// Convert string to blend mode
inline BlendMode stringToBlendMode(const std::string& str) {
    if (str == "Multiply") return BlendMode::Multiply;
    if (str == "Screen") return BlendMode::Screen;
    if (str == "Overlay") return BlendMode::Overlay;
    if (str == "Darken") return BlendMode::Darken;
    if (str == "Lighten") return BlendMode::Lighten;
    if (str == "Color Dodge") return BlendMode::ColorDodge;
    if (str == "Color Burn") return BlendMode::ColorBurn;
    if (str == "Hard Light") return BlendMode::HardLight;
    if (str == "Soft Light") return BlendMode::SoftLight;
    if (str == "Difference") return BlendMode::Difference;
    if (str == "Exclusion") return BlendMode::Exclusion;
    return BlendMode::Normal;
}

} // namespace spriteai::core::document
