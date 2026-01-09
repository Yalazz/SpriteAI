#include "spriteai/core/tools/builtin/BrushTool.h"
#include "spriteai/core/tools/ToolContext.h"
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
    m_lastPositions.clear();

    // Get symmetry settings from tool context
    auto* symmetry = m_toolContext ? m_toolContext->symmetry() : nullptr;

    if (symmetry && symmetry->enabled) {
        // Get all mirrored positions
        auto mirroredPoints = symmetry->getMirroredPoints(in.x, in.y);
        m_mirrorCount = static_cast<int>(mirroredPoints.size());

        // Begin a stroke for each mirrored position
        for (const auto& [mx, my] : mirroredPoints) {
            doc.beginStroke(m_rgba, m_width);
            doc.addPoint(mx, my, in.pressure);
            doc.endStroke();  // End immediately, we'll continue in the same stroke
            m_lastPositions.push_back({mx, my});
        }

        // Re-open the primary stroke for continuation
        doc.beginStroke(m_rgba, m_width);
        doc.addPoint(in.x, in.y, in.pressure);
    } else {
        // No symmetry - single stroke
        m_mirrorCount = 1;
        m_lastPositions.push_back({in.x, in.y});
        doc.beginStroke(m_rgba, m_width);
        doc.addPoint(in.x, in.y, in.pressure);
    }
}

void BrushTool::onPointerMove(spriteai::core::document::SpriteDocument& doc,
                              spriteai::core::command::CommandStack&,
                              const ToolInput& in)
{
    if (!m_drawing) return;

    auto* symmetry = m_toolContext ? m_toolContext->symmetry() : nullptr;

    if (symmetry && symmetry->enabled) {
        // Get all mirrored positions
        auto mirroredPoints = symmetry->getMirroredPoints(in.x, in.y);

        // Add points to all mirrored strokes
        for (size_t i = 0; i < mirroredPoints.size() && i < m_lastPositions.size(); ++i) {
            const auto& [mx, my] = mirroredPoints[i];
            const auto& [lastX, lastY] = m_lastPositions[i];

            // Apply spacing: only add point if far enough from last point
            float dx = mx - lastX;
            float dy = my - lastY;
            float distSq = dx * dx + dy * dy;
            float minDistSq = m_spacing * m_spacing;

            if (distSq >= minDistSq) {
                if (i == 0) {
                    // Primary stroke - add to current active stroke
                    doc.addPoint(mx, my, in.pressure);
                } else {
                    // Mirrored stroke - need to add separately
                    // We'll handle this by adding points to the last strokes
                    auto& strokes = doc.mutableStrokes();
                    if (!strokes.empty() && strokes.size() > i) {
                        size_t strokeIdx = strokes.size() - 1 - (mirroredPoints.size() - 1 - i);
                        if (strokeIdx < strokes.size()) {
                            strokes[strokeIdx].points.push_back(
                                spriteai::core::document::StrokePoint{mx, my, in.pressure}
                            );
                        }
                    }
                }
                m_lastPositions[i] = {mx, my};
            }
        }
    } else {
        // No symmetry - single stroke
        if (!m_lastPositions.empty()) {
            const auto& [lastX, lastY] = m_lastPositions[0];

            float dx = in.x - lastX;
            float dy = in.y - lastY;
            float distSq = dx * dx + dy * dy;
            float minDistSq = m_spacing * m_spacing;

            if (distSq >= minDistSq) {
                doc.addPoint(in.x, in.y, in.pressure);
                m_lastPositions[0] = {in.x, in.y};
            }
        }
    }
}

void BrushTool::onPointerUp(spriteai::core::document::SpriteDocument& doc,
                            spriteai::core::command::CommandStack& stack,
                            const ToolInput& in)
{
    if (!m_drawing) return;
    m_drawing = false;

    auto* symmetry = m_toolContext ? m_toolContext->symmetry() : nullptr;

    // Add final point and end the primary stroke
    doc.addPoint(in.x, in.y, in.pressure);
    doc.endStroke();

    // If symmetry is enabled, also add final points to mirrored strokes
    if (symmetry && symmetry->enabled) {
        auto mirroredPoints = symmetry->getMirroredPoints(in.x, in.y);
        auto& strokes = doc.mutableStrokes();

        for (size_t i = 1; i < mirroredPoints.size() && !strokes.empty(); ++i) {
            const auto& [mx, my] = mirroredPoints[i];
            size_t strokeIdx = strokes.size() - 1 - (mirroredPoints.size() - 1 - i);
            if (strokeIdx < strokes.size()) {
                strokes[strokeIdx].points.push_back(
                    spriteai::core::document::StrokePoint{mx, my, in.pressure}
                );
            }
        }
    }

    // Create commands for undo/redo for all strokes created
    auto& strokes = doc.mutableStrokes();
    if (!strokes.empty()) {
        // Only create command for the last stroke (simplification)
        const std::size_t idx = strokes.size() - 1;
        const auto strokeCopy = strokes[idx];
        stack.execute(doc, std::make_unique<spriteai::core::command::AddStrokeCommand>(strokeCopy, idx));
    }

    m_lastPositions.clear();
    m_mirrorCount = 1;
}

} // namespace spriteai::core::tools::builtin
