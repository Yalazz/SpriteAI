#pragma once
#include "spriteai/core/tools/Tool.h"
#include "spriteai/core/document/Stroke.h"

namespace spriteai::core::tools::builtin {

class PencilTool final : public Tool {
public:
    explicit PencilTool(std::uint32_t rgba = 0xFFFFFFFF);

    std::string id() const override { return "pencil"; }
    std::string displayName() const override { return "Pencil"; }

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
    std::uint32_t color() const { return m_rgba; }

private:
    std::uint32_t m_rgba;
    bool m_drawing = false;
    int m_lastPixelX = 0;
    int m_lastPixelY = 0;
};

} // namespace spriteai::core::tools::builtin
