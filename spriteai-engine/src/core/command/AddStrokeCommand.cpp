#include "spriteai/core/command/AddStrokeCommand.h"
#include "spriteai/core/document/SpriteDocument.h"

namespace spriteai::core::command {

AddStrokeCommand::AddStrokeCommand(spriteai::core::document::Stroke stroke)
    : m_stroke(std::move(stroke)) {}

void AddStrokeCommand::apply(spriteai::core::document::SpriteDocument& doc) {
    auto& strokes = doc.mutableStrokes();
    strokes.push_back(m_stroke);
    m_index = strokes.size() - 1;
}

void AddStrokeCommand::undo(spriteai::core::document::SpriteDocument& doc) {
    auto& strokes = doc.mutableStrokes();
    if (m_index >= strokes.size()) return;
    strokes.erase(strokes.begin() + static_cast<std::ptrdiff_t>(m_index));
    m_index = static_cast<std::size_t>(-1);
}

} // namespace spriteai::core::command
