#pragma once
#include <cstdint>
#include <vector>

namespace spriteai::core::document {

struct StrokePoint {
    float x = 0.0f;
    float y = 0.0f;
    float pressure = 1.0f;
};

struct Stroke {
    std::uint32_t rgba = 0xFFFFFFFF;
    float width = 1.0f;
    std::vector<StrokePoint> points;
};

} // namespace spriteai::core::document
