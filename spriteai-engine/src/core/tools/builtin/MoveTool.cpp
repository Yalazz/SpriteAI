#include "spriteai/core/tools/builtin/MoveTool.h"
#include "spriteai/core/tools/ToolContext.h"
#include "spriteai/core/selection/Selection.h"
#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/core/command/CommandStack.h"
#include "spriteai/core/command/Command.h"

namespace spriteai::core::tools::builtin {

namespace {

class MoveStrokesCommand : public spriteai::core::command::Command {
public:
    MoveStrokesCommand(std::vector<std::size_t> indices, float dx, float dy)
        : m_indices(std::move(indices)), m_dx(dx), m_dy(dy) {}

    void apply(spriteai::core::document::SpriteDocument& doc) override {
        auto& strokes = doc.mutableStrokes();
        for (auto idx : m_indices) {
            if (idx < strokes.size()) {
                for (auto& pt : strokes[idx].points) {
                    pt.x += m_dx;
                    pt.y += m_dy;
                }
            }
        }
    }

    void undo(spriteai::core::document::SpriteDocument& doc) override {
        auto& strokes = doc.mutableStrokes();
        for (auto idx : m_indices) {
            if (idx < strokes.size()) {
                for (auto& pt : strokes[idx].points) {
                    pt.x -= m_dx;
                    pt.y -= m_dy;
                }
            }
        }
    }

private:
    std::vector<std::size_t> m_indices;
    float m_dx;
    float m_dy;
};

}

void MoveTool::onPointerDown(spriteai::core::document::SpriteDocument& doc,
                             spriteai::core::command::CommandStack&,
                             const ToolInput& in)
{
    m_moving = true;
    m_lastX = in.x;
    m_lastY = in.y;
    m_totalDx = 0.0f;
    m_totalDy = 0.0f;
    m_affectedStrokeIndices.clear();

    const auto& strokes = doc.strokes();

    if (m_toolContext && m_toolContext->selection() &&
        m_toolContext->selection()->hasSelection()) {
        auto* sel = m_toolContext->selection();

        for (std::size_t i = 0; i < strokes.size(); ++i) {
            bool affected = false;
            for (const auto& pt : strokes[i].points) {
                if (sel->containsPoint(pt.x, pt.y)) {
                    affected = true;
                    break;
                }
            }
            if (affected) {
                m_affectedStrokeIndices.push_back(i);
            }
        }
    } else {
        for (std::size_t i = 0; i < strokes.size(); ++i) {
            m_affectedStrokeIndices.push_back(i);
        }
    }
}

void MoveTool::onPointerMove(spriteai::core::document::SpriteDocument& doc,
                             spriteai::core::command::CommandStack&,
                             const ToolInput& in)
{
    if (!m_moving) return;

    float dx = in.x - m_lastX;
    float dy = in.y - m_lastY;

    if (dx == 0.0f && dy == 0.0f) return;

    m_totalDx += dx;
    m_totalDy += dy;

    auto& strokes = doc.mutableStrokes();
    for (auto idx : m_affectedStrokeIndices) {
        if (idx < strokes.size()) {
            for (auto& pt : strokes[idx].points) {
                pt.x += dx;
                pt.y += dy;
            }
        }
    }

    if (m_toolContext && m_toolContext->selection()) {
        m_toolContext->selection()->translate(dx, dy);
    }

    m_lastX = in.x;
    m_lastY = in.y;
}

void MoveTool::onPointerUp(spriteai::core::document::SpriteDocument& doc,
                           spriteai::core::command::CommandStack& stack,
                           const ToolInput&)
{
    if (!m_moving) return;
    m_moving = false;

    if ((m_totalDx != 0.0f || m_totalDy != 0.0f) && !m_affectedStrokeIndices.empty()) {
        auto& strokes = doc.mutableStrokes();
        for (auto idx : m_affectedStrokeIndices) {
            if (idx < strokes.size()) {
                for (auto& pt : strokes[idx].points) {
                    pt.x -= m_totalDx;
                    pt.y -= m_totalDy;
                }
            }
        }

        stack.execute(doc, std::make_unique<MoveStrokesCommand>(
            m_affectedStrokeIndices, m_totalDx, m_totalDy));
    }

    m_affectedStrokeIndices.clear();
}

} // namespace spriteai::core::tools::builtin
