#include "spriteai/core/command/AddStrokeCommand.h"
#include "spriteai/core/document/SpriteDocument.h"

namespace spriteai::core::command {

    AddStrokeCommand::AddStrokeCommand(spriteai::core::document::Stroke stroke)
        : m_stroke(std::move(stroke))
    {}

    AddStrokeCommand::AddStrokeCommand(spriteai::core::document::Stroke stroke,
                                       std::size_t alreadyInsertedIndex)
        : m_stroke(std::move(stroke))
        , m_alreadyInsertedIndex(alreadyInsertedIndex)
    {}

    void AddStrokeCommand::apply(spriteai::core::document::SpriteDocument& doc) {
        auto& strokes = doc.mutableStrokes();

        // Stroke zaten doc içine eklendiyse tekrar ekleme.
        if (m_alreadyInsertedIndex.has_value()) {
            m_index = *m_alreadyInsertedIndex;
            // Stroke zaten doğru yerde mi kontrol et
            if (m_index < strokes.size()) {
                // Stroke zaten var, hiçbir şey yapma
                return;
            }
            // Index out of bounds ise, redo için normal insert yap
            // (undo yapılmış demektir)
        }

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
