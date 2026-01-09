#pragma once
#include "spriteai/core/tools/Tool.h"
#include <cstdint>

namespace spriteai::core::tools::builtin {

class FillTool final : public Tool {
public:
    explicit FillTool(std::uint32_t rgba = 0xFFFFFFFF);

    std::string id() const override { return "fill"; }
    std::string displayName() const override { return "Fill"; }

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

    void setTolerance(int tol) { m_tolerance = tol; }
    int tolerance() const { return m_tolerance; }

private:
    std::uint32_t m_rgba;
    int m_tolerance = 0;

    bool colorsMatch(std::uint32_t c1, std::uint32_t c2) const;
};

} // namespace spriteai::core::tools::builtin
