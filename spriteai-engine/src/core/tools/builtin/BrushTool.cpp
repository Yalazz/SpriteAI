#include "spriteai/core/tools/builtin/BrushTool.h"
#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/core/command/CommandStack.h"
#include "spriteai/core/command/AddStrokeCommand.h"

namespace spriteai::core::tools::builtin {

BrushTool::BrushTool(std::uint32_t rgba, float width) : m_rgba(rgba), m_width(width) {}

void BrushTool::onPointerDown(spriteai::core::document::SpriteDocument&,
                              spriteai::core::command::CommandStack&,
                              const ToolInput& in) {
    m_drawing = true;
    m_current = {};
    m_current.rgba = m_rgba;
    m_current.width = m_width;
    m_current.points.push_back({in.x, in.y, in.pressure});
}

void BrushTool::onPointerMove(spriteai::core::document::SpriteDocument&,
                              spriteai::core::command::CommandStack&,
                              const ToolInput& in) {
    if (!m_drawing) return;
    m_current.points.push_back({in.x, in.y, in.pressure});
}

void BrushTool::onPointerUp(spriteai::core::document::SpriteDocument& doc,
                            spriteai::core::command::CommandStack& stack,
                            const ToolInput& in) {
    if (!m_drawing) return;
    m_current.points.push_back({in.x, in.y, in.pressure});
    m_drawing = false;

    if (m_current.points.size() < 2) return;
    stack.execute(doc, std::make_unique<spriteai::core::command::AddStrokeCommand>(m_current));
}

} // namespace spriteai::core::tools::builtin
