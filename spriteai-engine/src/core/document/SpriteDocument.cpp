#include "spriteai/core/document/SpriteDocument.h"

namespace spriteai::core::document {

    void SpriteDocument::beginStroke(std::uint32_t rgba, float width) {
        m_strokes.emplace_back();
        m_strokes.back().rgba = rgba;
        m_strokes.back().width = width;
        m_active = &m_strokes.back();
    }

    void SpriteDocument::addPoint(float x, float y, float pressure) {
        if (!m_active) return;
        m_active->points.push_back(StrokePoint{ x, y, pressure });
    }

    void SpriteDocument::endStroke() {
        m_active = nullptr;
    }

    void SpriteDocument::addStroke(const Stroke& stroke) {
        m_strokes.push_back(stroke);
    }

    void SpriteDocument::removeLastStroke() {
        if (!m_strokes.empty()) {
            m_strokes.pop_back();
        }
    }

    void SpriteDocument::clear() {
        m_strokes.clear();
        m_active = nullptr;
    }

    const std::vector<Stroke>& SpriteDocument::strokes() const {
        return m_strokes;
    }

    std::vector<Stroke>& SpriteDocument::mutableStrokes() {
        return m_strokes;
    }

} // namespace spriteai::core::document
