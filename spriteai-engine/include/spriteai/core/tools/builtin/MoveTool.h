#pragma once
#include "spriteai/core/tools/Tool.h"
#include <vector>
#include <cstddef>

namespace spriteai::core::tools::builtin {

class MoveTool final : public Tool {
public:
    MoveTool() = default;

    std::string id() const override { return "move"; }
    std::string displayName() const override { return "Move"; }

    void onPointerDown(spriteai::core::document::SpriteDocument&,
                       spriteai::core::command::CommandStack&,
                       const ToolInput&) override;

    void onPointerMove(spriteai::core::document::SpriteDocument&,
                       spriteai::core::command::CommandStack&,
                       const ToolInput&) override;

    void onPointerUp(spriteai::core::document::SpriteDocument&,
                     spriteai::core::command::CommandStack&,
                     const ToolInput&) override;

private:
    bool m_moving = false;
    float m_lastX = 0.0f;
    float m_lastY = 0.0f;
    float m_totalDx = 0.0f;
    float m_totalDy = 0.0f;
    std::vector<std::size_t> m_affectedStrokeIndices;
};

} // namespace spriteai::core::tools::builtin
