#pragma once
#include "spriteai/core/tools/Tool.h"
#include "spriteai/core/document/Stroke.h"

namespace spriteai::core::tools::builtin {

class BrushTool final : public Tool {
public:
    BrushTool(std::uint32_t rgba = 0xFFFFFFFF, float width = 6.0f);

    std::string id() const override { return "brush"; }
    std::string displayName() const override { return "Brush"; }

    void onPointerDown(spriteai::core::document::SpriteDocument&,
                       spriteai::core::command::CommandStack&,
                       const ToolInput&) override;

    void onPointerMove(spriteai::core::document::SpriteDocument&,
                       spriteai::core::command::CommandStack&,
                       const ToolInput&) override;

    void onPointerUp(spriteai::core::document::SpriteDocument&,
                     spriteai::core::command::CommandStack&,
                     const ToolInput&) override;

    void setColor(std::uint32_t rgba) { m_rgba = rgba; }
    void setWidth(float w) { m_width = w; }

private:
    std::uint32_t m_rgba;
    float m_width;
    bool m_drawing = false;
    spriteai::core::document::Stroke m_current{};
};

} // namespace spriteai::core::tools::builtin
