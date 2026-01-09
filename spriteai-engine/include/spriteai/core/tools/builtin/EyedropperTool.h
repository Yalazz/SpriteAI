#pragma once
#include "spriteai/core/tools/Tool.h"
#include <cstdint>

namespace spriteai::core::tools::builtin {

class EyedropperTool final : public Tool {
public:
    EyedropperTool() = default;

    std::string id() const override { return "eyedropper"; }
    std::string displayName() const override { return "Eyedropper"; }

    void onPointerDown(spriteai::core::document::SpriteDocument&,
                       spriteai::core::command::CommandStack&,
                       const ToolInput&) override;

    void onPointerMove(spriteai::core::document::SpriteDocument&,
                       spriteai::core::command::CommandStack&,
                       const ToolInput&) override;

    void onPointerUp(spriteai::core::document::SpriteDocument&,
                     spriteai::core::command::CommandStack&,
                     const ToolInput&) override;

    std::uint32_t lastPickedColor() const { return m_lastColor; }

private:
    std::uint32_t m_lastColor = 0xFFFFFFFF;
    bool m_picking = false;

    void pickColor(const ToolInput& in);
};

} // namespace spriteai::core::tools::builtin
