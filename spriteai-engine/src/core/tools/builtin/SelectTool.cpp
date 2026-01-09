#include "spriteai/core/tools/builtin/SelectTool.h"
#include "spriteai/core/tools/ToolContext.h"
#include "spriteai/core/selection/Selection.h"
#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/core/command/CommandStack.h"

#include <cmath>
#include <algorithm>

namespace spriteai::core::tools::builtin {

void SelectTool::onPointerDown(spriteai::core::document::SpriteDocument&,
                               spriteai::core::command::CommandStack&,
                               const ToolInput& in)
{
    m_selecting = true;
    m_startX = in.x;
    m_startY = in.y;

    if (!m_toolContext || !m_toolContext->selection()) return;

    auto* sel = m_toolContext->selection();

    if (m_mode == SelectMode::Rectangle) {
        sel->setRect({m_startX, m_startY, 0.0f, 0.0f});
    } else {
        sel->beginFreeform(in.x, in.y);
    }
}

void SelectTool::onPointerMove(spriteai::core::document::SpriteDocument&,
                               spriteai::core::command::CommandStack&,
                               const ToolInput& in)
{
    if (!m_selecting) return;
    if (!m_toolContext || !m_toolContext->selection()) return;

    auto* sel = m_toolContext->selection();

    if (m_mode == SelectMode::Rectangle) {
        float x = std::min(m_startX, in.x);
        float y = std::min(m_startY, in.y);
        float w = std::abs(in.x - m_startX);
        float h = std::abs(in.y - m_startY);

        if (in.shift) {
            float size = std::max(w, h);
            w = size;
            h = size;
            if (in.x < m_startX) x = m_startX - size;
            if (in.y < m_startY) y = m_startY - size;
        }

        sel->setRect({x, y, w, h});
    } else {
        sel->addFreeformPoint(in.x, in.y);
    }
}

void SelectTool::onPointerUp(spriteai::core::document::SpriteDocument&,
                             spriteai::core::command::CommandStack&,
                             const ToolInput& in)
{
    if (!m_selecting) return;
    m_selecting = false;

    if (!m_toolContext || !m_toolContext->selection()) return;

    auto* sel = m_toolContext->selection();

    if (m_mode == SelectMode::Rectangle) {
        float x = std::min(m_startX, in.x);
        float y = std::min(m_startY, in.y);
        float w = std::abs(in.x - m_startX);
        float h = std::abs(in.y - m_startY);

        if (in.shift) {
            float size = std::max(w, h);
            w = size;
            h = size;
            if (in.x < m_startX) x = m_startX - size;
            if (in.y < m_startY) y = m_startY - size;
        }

        if (w < 1.0f && h < 1.0f) {
            sel->clear();
        } else {
            sel->setRect({x, y, w, h});
        }
    } else {
        sel->closeFreeform();
    }
}

} // namespace spriteai::core::tools::builtin
