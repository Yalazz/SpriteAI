#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include "spriteai/core/document/Stroke.h"
#include "spriteai/core/document/Layer.h"

namespace spriteai::core::document {

class SpriteDocument {
public:
    SpriteDocument();

    // -------- Interactive drawing (tool-side) - operates on active layer
    void beginStroke(std::uint32_t rgba, float width);
    void addPoint(float x, float y, float pressure = 1.0f);
    void endStroke();

    // -------- Command-based API (UNDO / REDO)
    void addStroke(const Stroke& stroke);
    void removeLastStroke();

    void clear();

    // -------- Layer Management
    int addLayer(const std::string& name = "");
    void deleteLayer(int index);
    void duplicateLayer(int index);
    void moveLayer(int from, int to);

    void setActiveLayer(int index);
    int activeLayerIndex() const { return m_activeLayer; }

    void setLayerVisible(int index, bool visible);
    void setLayerLocked(int index, bool locked);
    void setLayerOpacity(int index, float opacity);
    void setLayerBlendMode(int index, BlendMode mode);
    void setLayerName(int index, const std::string& name);

    // -------- Layer Access
    const std::vector<Layer>& layers() const { return m_layers; }
    std::vector<Layer>& mutableLayers() { return m_layers; }
    Layer* activeLayer();
    const Layer* activeLayer() const;
    int layerCount() const { return static_cast<int>(m_layers.size()); }

    // -------- Flattened strokes (for backward compatibility)
    // Returns all strokes from all visible layers, bottom to top
    std::vector<Stroke> allVisibleStrokes() const;

    // Legacy API (operates on active layer)
    const std::vector<Stroke>& strokes() const;
    std::vector<Stroke>& mutableStrokes();

private:
    std::vector<Layer> m_layers;
    int m_activeLayer = 0;
    Stroke* m_active = nullptr;
    int m_layerCounter = 1;
};

} // namespace spriteai::core::document
