#pragma once
#include <vector>
#include <cstdint>
#include "spriteai/core/document/Stroke.h"

namespace spriteai::engine::ai {

// Input: a stroke; Output: one or more strokes (e.g. cleaned line, stylized)
class IAIClient {
public:
    virtual ~IAIClient() = default;
    virtual std::vector<spriteai::core::document::Stroke>
    strokeToStrokes(const spriteai::core::document::Stroke& inStroke) = 0;
};

} // namespace spriteai::engine::ai
