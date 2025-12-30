#include "spriteai/core/tools/builtin/AITool.h"
#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/core/command/CommandStack.h"
#include "spriteai/core/command/AddStrokeCommand.h"
#include "spriteai/engine/ai/IAIClient.h"

namespace spriteai::core::tools::builtin {

AITool::AITool(std::string mode) : m_mode(std::move(mode)) {}

void AITool::onPointerDown(spriteai::core::document::SpriteDocument&,
                           spriteai::core::command::CommandStack&,
                           const ToolInput& in) {
    m_drawing = true;
    m_input = {};
    m_input.rgba = 0xFFFFFFFFu;
    m_input.width = 6.0f;
    m_input.points.push_back({in.x, in.y, in.pressure});
}

void AITool::onPointerMove(spriteai::core::document::SpriteDocument&,
                           spriteai::core::command::CommandStack&,
                           const ToolInput& in) {
    if (!m_drawing) return;
    m_input.points.push_back({in.x, in.y, in.pressure});
}

void AITool::onPointerUp(spriteai::core::document::SpriteDocument& doc,
                         spriteai::core::command::CommandStack& stack,
                         const ToolInput& in) {
    if (!m_drawing) return;
    m_input.points.push_back({in.x, in.y, in.pressure});
    m_drawing = false;

    if (!m_client || m_input.points.size() < 2) return;

    auto out = m_client->strokeToStrokes(m_input);
    for (auto& s : out) {
        if (s.points.size() < 2) continue;
        stack.execute(doc, std::make_unique<spriteai::core::command::AddStrokeCommand>(std::move(s)));
    }
}

} // namespace spriteai::core::tools::builtin
