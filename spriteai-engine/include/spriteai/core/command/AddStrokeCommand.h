#pragma once
#include <cstddef>
#include "spriteai/core/command/Command.h"
#include "spriteai/core/document/Stroke.h"

namespace spriteai::core::command {

class AddStrokeCommand final : public Command {
public:
    explicit AddStrokeCommand(spriteai::core::document::Stroke stroke);

    void apply(spriteai::core::document::SpriteDocument& doc) override;
    void undo(spriteai::core::document::SpriteDocument& doc) override;

private:
    spriteai::core::document::Stroke m_stroke;
    // index where stroke was inserted (for undo)
    std::size_t m_index = static_cast<std::size_t>(-1);
};

} // namespace spriteai::core::command
