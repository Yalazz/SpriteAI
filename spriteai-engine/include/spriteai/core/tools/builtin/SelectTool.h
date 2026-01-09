#pragma once
#include "spriteai/core/tools/Tool.h"

namespace spriteai::core::tools::builtin {

enum class SelectMode {
    Rectangle,
    Freeform
};

class SelectTool final : public Tool {
public:
    SelectTool() = default;

    std::string id() const override { return "select"; }
    std::string displayName() const override { return "Select"; }

    void onPointerDown(spriteai::core::document::SpriteDocument&,
                       spriteai::core::command::CommandStack&,
                       const ToolInput&) override;

    void onPointerMove(spriteai::core::document::SpriteDocument&,
                       spriteai::core::command::CommandStack&,
                       const ToolInput&) override;

    void onPointerUp(spriteai::core::document::SpriteDocument&,
                     spriteai::core::command::CommandStack&,
                     const ToolInput&) override;

    void setMode(SelectMode mode) { m_mode = mode; }
    SelectMode mode() const { return m_mode; }

private:
    SelectMode m_mode = SelectMode::Rectangle;
    bool m_selecting = false;
    float m_startX = 0.0f;
    float m_startY = 0.0f;
};

} // namespace spriteai::core::tools::builtin
