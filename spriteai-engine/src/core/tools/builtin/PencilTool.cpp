#include "spriteai/core/tools/builtin/PencilTool.h"
#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/core/command/CommandStack.h"
#include "spriteai/core/command/AddStrokeCommand.h"

#include <cmath>

namespace spriteai::core::tools::builtin {

PencilTool::PencilTool(std::uint32_t rgba)
    : m_rgba(rgba)
{}

void PencilTool::onPointerDown(spriteai::core::document::SpriteDocument& doc,
                               spriteai::core::command::CommandStack&,
                               const ToolInput& in)
{
    m_drawing = true;

    int px = static_cast<int>(std::floor(in.x));
    int py = static_cast<int>(std::floor(in.y));
    m_lastPixelX = px;
    m_lastPixelY = py;

    doc.beginStroke(m_rgba, 1.0f);
    doc.addPoint(static_cast<float>(px) + 0.5f,
                 static_cast<float>(py) + 0.5f,
                 1.0f);
}

void PencilTool::onPointerMove(spriteai::core::document::SpriteDocument& doc,
                               spriteai::core::command::CommandStack&,
                               const ToolInput& in)
{
    if (!m_drawing) return;

    int px = static_cast<int>(std::floor(in.x));
    int py = static_cast<int>(std::floor(in.y));

    if (px == m_lastPixelX && py == m_lastPixelY) return;

    int x0 = m_lastPixelX;
    int y0 = m_lastPixelY;
    int x1 = px;
    int y1 = py;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x0 != m_lastPixelX || y0 != m_lastPixelY) {
            doc.addPoint(static_cast<float>(x0) + 0.5f,
                         static_cast<float>(y0) + 0.5f,
                         1.0f);
        }

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

    m_lastPixelX = px;
    m_lastPixelY = py;
}

void PencilTool::onPointerUp(spriteai::core::document::SpriteDocument& doc,
                             spriteai::core::command::CommandStack& stack,
                             const ToolInput&)
{
    if (!m_drawing) return;
    m_drawing = false;

    doc.endStroke();

    auto& strokes = doc.mutableStrokes();
    if (strokes.empty()) return;

    const std::size_t idx = strokes.size() - 1;
    const auto strokeCopy = strokes[idx];

    stack.execute(doc, std::make_unique<spriteai::core::command::AddStrokeCommand>(strokeCopy, idx));
}

} // namespace spriteai::core::tools::builtin
