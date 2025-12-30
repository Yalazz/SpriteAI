#pragma once
#include <cstdint>
#include <string>

namespace spriteai::engine {

// Simple engine-side theme payload. Editor loads JSON and pushes values here.
struct ColorRGBA8 { std::uint8_t r=0,g=0,b=0,a=255; };

struct Theme {
    std::string name = "Default";
    ColorRGBA8 background{};
    ColorRGBA8 canvas{};
    ColorRGBA8 gridMajor{};
    ColorRGBA8 gridMinor{};
    ColorRGBA8 stroke{};
    ColorRGBA8 guide{};
    ColorRGBA8 symmetry{};
};

} // namespace spriteai::engine
