#include "spriteai/core/tools/builtin/BrushTool.h"
#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/core/command/CommandStack.h"
#include "spriteai/core/command/AddStrokeCommand.h"

namespace spriteai::core::tools::builtin {

    BrushTool::BrushTool(std::uint32_t rgba, float width)
        : m_rgba(rgba), m_width(width)
    {}

    void BrushTool::onPointerDown(spriteai::core::document::SpriteDocument& doc,
                                  spriteai::core::command::CommandStack&,
                                  const ToolInput& in)
    {
        m_drawing = true;
        m_lastX = in.x;
        m_lastY = in.y;

        // LIVE: document içine stroke aç
        doc.beginStroke(m_rgba, m_width);
        doc.addPoint(in.x, in.y, in.pressure);
    }

    void BrushTool::onPointerMove(spriteai::core::document::SpriteDocument& doc,
                                  spriteai::core::command::CommandStack&,
                                  const ToolInput& in)
    {
        if (!m_drawing) return;

        // Apply spacing: only add point if far enough from last point
        float dx = in.x - m_lastX;
        float dy = in.y - m_lastY;
        float distSq = dx * dx + dy * dy;
        float minDistSq = m_spacing * m_spacing;

        if (distSq >= minDistSq) {
            doc.addPoint(in.x, in.y, in.pressure);
            m_lastX = in.x;
            m_lastY = in.y;
        }
    }

    void BrushTool::onPointerUp(spriteai::core::document::SpriteDocument& doc,
                                spriteai::core::command::CommandStack& stack,
                                const ToolInput& in)
    {
        if (!m_drawing) return;
        m_drawing = false;

        // LIVE: son nokta + stroke bitir
        doc.addPoint(in.x, in.y, in.pressure);
        doc.endStroke();

        // Document'e eklenmiş son stroke'u command'a dönüştür (undo/redo için)
        auto& strokes = doc.mutableStrokes();
        if (strokes.empty()) return;

        const std::size_t idx = strokes.size() - 1;
        const auto strokeCopy = strokes[idx]; // command kendi kopyasını tutsun

        // "alreadyInsertedIndex" ile apply tekrar eklemeyecek
        stack.execute(doc, std::make_unique<spriteai::core::command::AddStrokeCommand>(strokeCopy, idx));
    }

} // namespace spriteai::core::tools::builtin
