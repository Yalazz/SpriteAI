#pragma once

namespace spriteai::core::document {
    class SpriteDocument;
}

namespace spriteai::core::command {

    class Command {
    public:
        virtual ~Command() = default;

        // Apply change to document (forward)
        virtual void apply(document::SpriteDocument& doc) = 0;

        // Revert change from document (backward)
        virtual void undo(document::SpriteDocument& doc) = 0;
    };

} // namespace spriteai::core::command
