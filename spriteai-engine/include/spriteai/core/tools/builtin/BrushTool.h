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
    void setSpacing(float s) { m_spacing = s; }

private:
    std::uint32_t m_rgba;
    float m_width;
    float m_spacing = 0.25f;
    bool m_drawing = false;
    float m_lastX = 0.0f;
    float m_lastY = 0.0f;
    spriteai::core::document::Stroke m_current{};
};

} // namespace spriteai::core::tools::builtin
