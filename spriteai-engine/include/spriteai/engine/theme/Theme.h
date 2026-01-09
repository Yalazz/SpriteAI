#pragma once
#include <cstdint>
#include <string>

namespace spriteai::engine {

// Simple engine-side theme payload. Editor loads JSON and pushes values here.
struct ColorRGBA8 {
    std::uint8_t r=0, g=0, b=0, a=255;

    std::string toHex() const {
        char buf[8];
        snprintf(buf, sizeof(buf), "#%02X%02X%02X", r, g, b);
        return std::string(buf);
    }

    std::string toRgba() const {
        char buf[32];
        snprintf(buf, sizeof(buf), "rgba(%d,%d,%d,%d)", r, g, b, a);
        return std::string(buf);
    }
};

struct Theme {
    std::string name = "Default";

    // Canvas colors (used by renderer)
    ColorRGBA8 background{};
    ColorRGBA8 canvas{};
    ColorRGBA8 gridMajor{};
    ColorRGBA8 gridMinor{};
    ColorRGBA8 stroke{};
    ColorRGBA8 guide{};
    ColorRGBA8 symmetry{};

    // UI colors (used by Qt stylesheet generator)
    ColorRGBA8 uiBackground{26, 26, 46, 255};      // Main window background
    ColorRGBA8 uiSurface{22, 33, 62, 255};         // Panels, docks
    ColorRGBA8 uiSurfaceHover{15, 52, 96, 255};    // Hover state
    ColorRGBA8 uiPrimary{233, 69, 96, 255};        // Primary accent (buttons, selection)
    ColorRGBA8 uiPrimaryHover{200, 50, 80, 255};   // Primary hover
    ColorRGBA8 uiSecondary{78, 205, 196, 255};     // Secondary accent
    ColorRGBA8 uiText{232, 232, 232, 255};         // Primary text
    ColorRGBA8 uiTextSecondary{160, 160, 180, 255};// Secondary text
    ColorRGBA8 uiTextDisabled{100, 100, 120, 255}; // Disabled text
    ColorRGBA8 uiBorder{15, 52, 96, 255};          // Borders
    ColorRGBA8 uiSuccess{46, 204, 113, 255};       // Success state
    ColorRGBA8 uiWarning{241, 196, 15, 255};       // Warning state
    ColorRGBA8 uiError{231, 76, 60, 255};          // Error state

    // UI metrics
    int borderRadius = 6;
    int spacing = 8;
    std::string fontFamily = "Segoe UI";
    int fontSize = 10;
};

} // namespace spriteai::engine
