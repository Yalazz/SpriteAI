#include "spriteai/core/command/CommandStack.h"
#include "spriteai/core/command/Command.h"

namespace spriteai::core::command {

void CommandStack::execute(spriteai::core::document::SpriteDocument& doc, std::unique_ptr<Command> cmd) {
    if (!cmd) return;
    cmd->apply(doc);
    m_done.push_back(std::move(cmd));
    m_undone.clear();
}

void CommandStack::undo(spriteai::core::document::SpriteDocument& doc) {
    if (m_done.empty()) return;
    auto cmd = std::move(m_done.back());
    m_done.pop_back();
    cmd->undo(doc);
    m_undone.push_back(std::move(cmd));
}

void CommandStack::redo(spriteai::core::document::SpriteDocument& doc) {
    if (m_undone.empty()) return;
    auto cmd = std::move(m_undone.back());
    m_undone.pop_back();
    cmd->apply(doc);
    m_done.push_back(std::move(cmd));
}

void CommandStack::clear() {
    m_done.clear();
    m_undone.clear();
}

} // namespace spriteai::core::command
