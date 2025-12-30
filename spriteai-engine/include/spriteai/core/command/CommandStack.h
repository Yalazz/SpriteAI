#pragma once
#include <memory>
#include <vector>

#include "spriteai/core/command/Command.h" // ✅ ZORUNLU

namespace spriteai::core::document {
    class SpriteDocument;
}

namespace spriteai::core::command {

    class CommandStack {
    public:
        void execute(document::SpriteDocument& doc,
                     std::unique_ptr<Command> cmd);

        void undo(document::SpriteDocument& doc);
        void redo(document::SpriteDocument& doc);
        void clear();

    private:
        std::vector<std::unique_ptr<Command>> m_done;
        std::vector<std::unique_ptr<Command>> m_undone;
    };

} // namespace spriteai::core::command
