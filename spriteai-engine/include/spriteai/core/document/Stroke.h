#pragma once
#include <cstdint>
#include <vector>

namespace spriteai::core::document {

enum class StrokeType : std::uint8_t {
    Line = 0,
    Fill = 1,
    Rectangle = 2
};

struct StrokePoint {
    float x = 0.0f;
    float y = 0.0f;
    float pressure = 1.0f;
};

struct Stroke {
    StrokeType type = StrokeType::Line;
    std::uint32_t rgba = 0xFFFFFFFF;
    float width = 1.0f;
    std::vector<StrokePoint> points;

    bool isFill() const { return type == StrokeType::Fill; }
    bool isLine() const { return type == StrokeType::Line; }
    bool isRectangle() const { return type == StrokeType::Rectangle; }
};

} // namespace spriteai::core::document
