#pragma once
#include <cstdint>
#include <vector>
#include <optional>
#include <algorithm>

namespace spriteai::core::selection {

struct Rect {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    bool contains(float px, float py) const {
        return px >= x && px <= x + width &&
               py >= y && py <= y + height;
    }

    bool isEmpty() const {
        return width <= 0.0f || height <= 0.0f;
    }

    Rect normalized() const {
        Rect r = *this;
        if (r.width < 0) {
            r.x += r.width;
            r.width = -r.width;
        }
        if (r.height < 0) {
            r.y += r.height;
            r.height = -r.height;
        }
        return r;
    }
};

enum class SelectionType {
    None,
    Rectangle,
    Freeform
};

class Selection {
public:
    Selection() = default;

    void clear() {
        m_type = SelectionType::None;
        m_rect = {};
        m_freeformPoints.clear();
    }

    void setRect(const Rect& rect) {
        m_type = SelectionType::Rectangle;
        m_rect = rect.normalized();
        m_freeformPoints.clear();
    }

    void beginFreeform(float x, float y) {
        m_type = SelectionType::Freeform;
        m_freeformPoints.clear();
        m_freeformPoints.push_back({x, y});
        m_rect = {};
    }

    void addFreeformPoint(float x, float y) {
        if (m_type != SelectionType::Freeform) return;
        m_freeformPoints.push_back({x, y});
        updateBoundsFromFreeform();
    }

    void closeFreeform() {
        if (m_type == SelectionType::Freeform && !m_freeformPoints.empty()) {
            m_freeformPoints.push_back(m_freeformPoints.front());
        }
    }

    bool hasSelection() const {
        return m_type != SelectionType::None && !m_rect.isEmpty();
    }

    SelectionType type() const { return m_type; }
    const Rect& bounds() const { return m_rect; }
    const std::vector<std::pair<float, float>>& freeformPoints() const {
        return m_freeformPoints;
    }

    bool containsPoint(float x, float y) const {
        if (m_type == SelectionType::None) return false;
        if (m_type == SelectionType::Rectangle) {
            return m_rect.contains(x, y);
        }
        // For freeform, use point-in-polygon test
        return pointInPolygon(x, y);
    }

    void translate(float dx, float dy) {
        m_rect.x += dx;
        m_rect.y += dy;
        for (auto& pt : m_freeformPoints) {
            pt.first += dx;
            pt.second += dy;
        }
    }

private:
    SelectionType m_type = SelectionType::None;
    Rect m_rect{};
    std::vector<std::pair<float, float>> m_freeformPoints;

    void updateBoundsFromFreeform() {
        if (m_freeformPoints.empty()) {
            m_rect = {};
            return;
        }
        float minX = m_freeformPoints[0].first;
        float maxX = minX;
        float minY = m_freeformPoints[0].second;
        float maxY = minY;
        for (const auto& pt : m_freeformPoints) {
            minX = std::min(minX, pt.first);
            maxX = std::max(maxX, pt.first);
            minY = std::min(minY, pt.second);
            maxY = std::max(maxY, pt.second);
        }
        m_rect = {minX, minY, maxX - minX, maxY - minY};
    }

    bool pointInPolygon(float x, float y) const {
        if (m_freeformPoints.size() < 3) return false;
        bool inside = false;
        std::size_t n = m_freeformPoints.size();
        for (std::size_t i = 0, j = n - 1; i < n; j = i++) {
            float xi = m_freeformPoints[i].first;
            float yi = m_freeformPoints[i].second;
            float xj = m_freeformPoints[j].first;
            float yj = m_freeformPoints[j].second;
            if (((yi > y) != (yj > y)) &&
                (x < (xj - xi) * (y - yi) / (yj - yi) + xi)) {
                inside = !inside;
            }
        }
        return inside;
    }
};

} // namespace spriteai::core::selection
