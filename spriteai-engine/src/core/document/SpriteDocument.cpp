#include "spriteai/core/document/SpriteDocument.h"
#include <algorithm>

namespace spriteai::core::document {

SpriteDocument::SpriteDocument() {
    // Create default background layer
    m_layers.emplace_back("Background");
    m_activeLayer = 0;
}

void SpriteDocument::beginStroke(std::uint32_t rgba, float width) {
    if (m_activeLayer < 0 || m_activeLayer >= static_cast<int>(m_layers.size())) return;

    Layer& layer = m_layers[m_activeLayer];
    if (layer.locked) return;

    layer.strokes.emplace_back();
    layer.strokes.back().rgba = rgba;
    layer.strokes.back().width = width;
    m_active = &layer.strokes.back();
}

void SpriteDocument::addPoint(float x, float y, float pressure) {
    if (!m_active) return;
    m_active->points.push_back(StrokePoint{ x, y, pressure });
}

void SpriteDocument::endStroke() {
    m_active = nullptr;
}

void SpriteDocument::addStroke(const Stroke& stroke) {
    if (m_activeLayer < 0 || m_activeLayer >= static_cast<int>(m_layers.size())) return;

    Layer& layer = m_layers[m_activeLayer];
    if (layer.locked) return;

    layer.strokes.push_back(stroke);
}

void SpriteDocument::removeLastStroke() {
    if (m_activeLayer < 0 || m_activeLayer >= static_cast<int>(m_layers.size())) return;

    Layer& layer = m_layers[m_activeLayer];
    if (!layer.strokes.empty()) {
        layer.strokes.pop_back();
    }
}

void SpriteDocument::clear() {
    for (auto& layer : m_layers) {
        layer.strokes.clear();
    }
    m_active = nullptr;
}

// -------- Layer Management

int SpriteDocument::addLayer(const std::string& name) {
    m_layerCounter++;
    std::string layerName = name.empty() ? "Layer " + std::to_string(m_layerCounter) : name;

    // Insert at top (index 0 is top in visual order, but we store bottom-to-top)
    // Actually, let's keep it simple: insert at the end and make it the active layer
    m_layers.emplace_back(layerName);
    int newIndex = static_cast<int>(m_layers.size()) - 1;
    m_activeLayer = newIndex;
    return newIndex;
}

void SpriteDocument::deleteLayer(int index) {
    if (index < 0 || index >= static_cast<int>(m_layers.size())) return;
    if (m_layers.size() <= 1) return; // Keep at least one layer

    m_layers.erase(m_layers.begin() + index);

    // Adjust active layer index
    if (m_activeLayer >= static_cast<int>(m_layers.size())) {
        m_activeLayer = static_cast<int>(m_layers.size()) - 1;
    }
}

void SpriteDocument::duplicateLayer(int index) {
    if (index < 0 || index >= static_cast<int>(m_layers.size())) return;

    Layer copy = m_layers[index];
    copy.name += " copy";

    // Insert after the original
    m_layers.insert(m_layers.begin() + index + 1, copy);
    m_activeLayer = index + 1;
}

void SpriteDocument::moveLayer(int from, int to) {
    if (from < 0 || from >= static_cast<int>(m_layers.size())) return;
    if (to < 0 || to >= static_cast<int>(m_layers.size())) return;
    if (from == to) return;

    Layer layer = std::move(m_layers[from]);
    m_layers.erase(m_layers.begin() + from);
    m_layers.insert(m_layers.begin() + to, std::move(layer));

    // Update active layer if it was moved
    if (m_activeLayer == from) {
        m_activeLayer = to;
    } else if (from < m_activeLayer && to >= m_activeLayer) {
        m_activeLayer--;
    } else if (from > m_activeLayer && to <= m_activeLayer) {
        m_activeLayer++;
    }
}

void SpriteDocument::setActiveLayer(int index) {
    if (index >= 0 && index < static_cast<int>(m_layers.size())) {
        m_activeLayer = index;
    }
}

void SpriteDocument::setLayerVisible(int index, bool visible) {
    if (index >= 0 && index < static_cast<int>(m_layers.size())) {
        m_layers[index].visible = visible;
    }
}

void SpriteDocument::setLayerLocked(int index, bool locked) {
    if (index >= 0 && index < static_cast<int>(m_layers.size())) {
        m_layers[index].locked = locked;
    }
}

void SpriteDocument::setLayerOpacity(int index, float opacity) {
    if (index >= 0 && index < static_cast<int>(m_layers.size())) {
        m_layers[index].opacity = std::clamp(opacity, 0.0f, 1.0f);
    }
}

void SpriteDocument::setLayerBlendMode(int index, BlendMode mode) {
    if (index >= 0 && index < static_cast<int>(m_layers.size())) {
        m_layers[index].blendMode = mode;
    }
}

void SpriteDocument::setLayerName(int index, const std::string& name) {
    if (index >= 0 && index < static_cast<int>(m_layers.size())) {
        m_layers[index].name = name;
    }
}

Layer* SpriteDocument::activeLayer() {
    if (m_activeLayer >= 0 && m_activeLayer < static_cast<int>(m_layers.size())) {
        return &m_layers[m_activeLayer];
    }
    return nullptr;
}

const Layer* SpriteDocument::activeLayer() const {
    if (m_activeLayer >= 0 && m_activeLayer < static_cast<int>(m_layers.size())) {
        return &m_layers[m_activeLayer];
    }
    return nullptr;
}

std::vector<Stroke> SpriteDocument::allVisibleStrokes() const {
    std::vector<Stroke> result;

    // Iterate layers from bottom to top
    for (const auto& layer : m_layers) {
        if (!layer.visible) continue;

        for (const auto& stroke : layer.strokes) {
            Stroke s = stroke;
            // Apply layer opacity to stroke alpha
            if (layer.opacity < 1.0f) {
                std::uint8_t alpha = (s.rgba >> 24) & 0xFF;
                alpha = static_cast<std::uint8_t>(alpha * layer.opacity);
                s.rgba = (static_cast<std::uint32_t>(alpha) << 24) | (s.rgba & 0x00FFFFFF);
            }
            result.push_back(s);
        }
    }

    return result;
}

// Legacy API - operates on active layer
const std::vector<Stroke>& SpriteDocument::strokes() const {
    static std::vector<Stroke> empty;
    if (m_activeLayer >= 0 && m_activeLayer < static_cast<int>(m_layers.size())) {
        return m_layers[m_activeLayer].strokes;
    }
    return empty;
}

std::vector<Stroke>& SpriteDocument::mutableStrokes() {
    static std::vector<Stroke> empty;
    if (m_activeLayer >= 0 && m_activeLayer < static_cast<int>(m_layers.size())) {
        return m_layers[m_activeLayer].strokes;
    }
    return empty;
}

} // namespace spriteai::core::document
