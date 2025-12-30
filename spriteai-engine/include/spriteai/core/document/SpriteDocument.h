#pragma once
#include <vector>
#include <cstdint>
#include "spriteai/core/document/Stroke.h"

namespace spriteai::core::document {

// "Document" ismiyle çakışma olmaması için: SpriteDocument.
class SpriteDocument {
public:
    void beginStroke(std::uint32_t rgba, float width);
    void addPoint(float x, float y, float pressure = 1.0f);
    void endStroke();

    void clear();

    const std::vector<Stroke>& strokes() const;
        std::vector<Stroke>& mutableStrokes();

private:
    std::vector<Stroke> m_strokes;
    Stroke* m_active = nullptr;
};

} // namespace spriteai::core::document
