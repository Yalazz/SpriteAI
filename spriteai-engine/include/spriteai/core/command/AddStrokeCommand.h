#pragma once
#include <cstddef>
#include <optional>
#include "spriteai/core/command/Command.h"
#include "spriteai/core/document/Stroke.h"

namespace spriteai::core::document { class SpriteDocument; }

namespace spriteai::core::command {

    class AddStrokeCommand final : public Command {
    public:
        // Normal kullanım: stroke daha document'e eklenmemişse
        explicit AddStrokeCommand(spriteai::core::document::Stroke stroke);

        // Live çizimde: stroke document'e zaten eklendi (index belli)
        AddStrokeCommand(spriteai::core::document::Stroke stroke, std::size_t alreadyInsertedIndex);

        void apply(spriteai::core::document::SpriteDocument& doc) override;
        void undo(spriteai::core::document::SpriteDocument& doc) override;

    private:
        spriteai::core::document::Stroke m_stroke{};
        std::optional<std::size_t> m_alreadyInsertedIndex; // varsa apply tekrar eklemez
        std::size_t m_index = static_cast<std::size_t>(-1);
    };

} // namespace spriteai::core::command
