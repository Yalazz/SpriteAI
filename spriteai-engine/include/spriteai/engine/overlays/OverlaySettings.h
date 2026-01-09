// spriteai-engine/include/spriteai/engine/overlays/OverlaySettings.h
#pragma once
#include <vector>
#include <cmath>

namespace spriteai::engine::overlays {

struct GridSettings {
    bool enabled = true;
    float minorStep = 16.0f;
    float majorStep = 64.0f;
    float opacity = 0.35f;
};

enum class SymmetryType {
    None = 0,
    Vertical,       // Mirror left/right
    Horizontal,     // Mirror up/down
    Both,           // Mirror both axes (4-way)
    Radial4,        // 4-way rotational
    Radial6,        // 6-way rotational
    Radial8         // 8-way rotational
};

// Keep old enum for backward compatibility
enum class SymmetryAxis { Vertical, Horizontal };

struct SymmetrySettings {
    bool enabled = false;
    SymmetryType type = SymmetryType::Vertical;
    SymmetryAxis axis = SymmetryAxis::Vertical;  // Legacy
    float axisX = 0.0f;  // Center X for symmetry
    float axisY = 0.0f;  // Center Y for symmetry
    bool drawAxisLine = true;
    bool drawCenterPoint = true;
    float lineOpacity = 0.85f;

    // Calculate mirrored points based on symmetry type
    std::vector<std::pair<float, float>> getMirroredPoints(float x, float y) const {
        std::vector<std::pair<float, float>> points;

        if (!enabled) {
            points.push_back({x, y});
            return points;
        }

        switch (type) {
            case SymmetryType::None:
                points.push_back({x, y});
                break;

            case SymmetryType::Vertical:
                points.push_back({x, y});
                points.push_back({2.0f * axisX - x, y});
                break;

            case SymmetryType::Horizontal:
                points.push_back({x, y});
                points.push_back({x, 2.0f * axisY - y});
                break;

            case SymmetryType::Both:
                points.push_back({x, y});
                points.push_back({2.0f * axisX - x, y});
                points.push_back({x, 2.0f * axisY - y});
                points.push_back({2.0f * axisX - x, 2.0f * axisY - y});
                break;

            case SymmetryType::Radial4:
            case SymmetryType::Radial6:
            case SymmetryType::Radial8: {
                int segments = (type == SymmetryType::Radial4) ? 4 :
                               (type == SymmetryType::Radial6) ? 6 : 8;

                float dx = x - axisX;
                float dy = y - axisY;
                float angle = std::atan2(dy, dx);
                float dist = std::sqrt(dx * dx + dy * dy);

                float angleStep = 2.0f * 3.14159265f / segments;

                for (int i = 0; i < segments; ++i) {
                    float newAngle = angle + i * angleStep;
                    float newX = axisX + dist * std::cos(newAngle);
                    float newY = axisY + dist * std::sin(newAngle);
                    points.push_back({newX, newY});

                    // Also add mirrored version for full symmetry
                    if (i % 2 == 0) {
                        float mirrorAngle = -angle + i * angleStep;
                        float mirrorX = axisX + dist * std::cos(mirrorAngle);
                        float mirrorY = axisY + dist * std::sin(mirrorAngle);
                        points.push_back({mirrorX, mirrorY});
                    }
                }
                break;
            }
        }

        return points;
    }
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
