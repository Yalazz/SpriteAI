#pragma once
#include "spriteai/core/tools/Tool.h"
#include "spriteai/core/tools/builtin/BrushTool.h"

namespace spriteai::core::tools::builtin {

    class EraserTool final : public Tool {
    public:
        explicit EraserTool(float width = 18.0f)
            : m_brush(0x00000000u, width) {}

        std::string id() const override { return "eraser"; }
        std::string displayName() const override { return "Eraser"; }

        void onPointerDown(spriteai::core::document::SpriteDocument& doc,
                           spriteai::core::command::CommandStack& stack,
                           const ToolInput& in) override {
            m_brush.onPointerDown(doc, stack, in);
        }

        void onPointerMove(spriteai::core::document::SpriteDocument& doc,
                           spriteai::core::command::CommandStack& stack,
                           const ToolInput& in) override {
            m_brush.onPointerMove(doc, stack, in);
        }

        void onPointerUp(spriteai::core::document::SpriteDocument& doc,
                         spriteai::core::command::CommandStack& stack,
                         const ToolInput& in) override {
            m_brush.onPointerUp(doc, stack, in);
        }

    private:
        BrushTool m_brush;
    };

} // namespace spriteai::core::tools::builtin
