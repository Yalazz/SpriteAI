#include "spriteai/core/tools/builtin/FillTool.h"
#include "spriteai/core/tools/ToolContext.h"
#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/core/document/Stroke.h"
#include "spriteai/core/command/CommandStack.h"
#include "spriteai/core/command/AddStrokeCommand.h"

#include <cmath>
#include <queue>
#include <vector>
#include <unordered_set>

namespace spriteai::core::tools::builtin {

FillTool::FillTool(std::uint32_t rgba)
    : m_rgba(rgba)
{}

bool FillTool::colorsMatch(std::uint32_t c1, std::uint32_t c2) const {
    if (m_tolerance == 0) {
        return c1 == c2;
    }

    auto r1 = static_cast<int>((c1 >> 24) & 0xFF);
    auto g1 = static_cast<int>((c1 >> 16) & 0xFF);
    auto b1 = static_cast<int>((c1 >> 8) & 0xFF);

    auto r2 = static_cast<int>((c2 >> 24) & 0xFF);
    auto g2 = static_cast<int>((c2 >> 16) & 0xFF);
    auto b2 = static_cast<int>((c2 >> 8) & 0xFF);

    return std::abs(r1 - r2) <= m_tolerance &&
           std::abs(g1 - g2) <= m_tolerance &&
           std::abs(b1 - b2) <= m_tolerance;
}

void FillTool::onPointerDown(spriteai::core::document::SpriteDocument& doc,
                             spriteai::core::command::CommandStack& stack,
                             const ToolInput& in)
{
    if (!m_toolContext) return;

    const auto& sampler = m_toolContext->pixelSampler();
    if (!sampler.sampleAt || sampler.bufferWidth <= 0 || sampler.bufferHeight <= 0) {
        return;
    }

    auto targetColorOpt = sampler.sampleAt(in.x, in.y);
    if (!targetColorOpt) return;

    std::uint32_t targetColor = *targetColorOpt;

    if (colorsMatch(targetColor, m_rgba)) return;

    int startX = static_cast<int>(std::floor(in.x));
    int startY = static_cast<int>(std::floor(in.y));

    if (startX < 0 || startX >= sampler.bufferWidth ||
        startY < 0 || startY >= sampler.bufferHeight) {
        return;
    }

    std::vector<std::pair<int, int>> filledPixels;
    std::vector<bool> visited(static_cast<size_t>(sampler.bufferWidth) *
                              static_cast<size_t>(sampler.bufferHeight), false);

    std::queue<std::pair<int, int>> queue;
    queue.push({startX, startY});
    visited[static_cast<size_t>(startY) * static_cast<size_t>(sampler.bufferWidth) +
            static_cast<size_t>(startX)] = true;

    const int maxPixels = 100000;
    int pixelCount = 0;

    while (!queue.empty() && pixelCount < maxPixels) {
        auto [x, y] = queue.front();
        queue.pop();

        auto colorOpt = sampler.sampleAt(static_cast<float>(x) + 0.5f,
                                         static_cast<float>(y) + 0.5f);
        if (!colorOpt || !colorsMatch(*colorOpt, targetColor)) {
            continue;
        }

        filledPixels.push_back({x, y});
        ++pixelCount;

        const int dx[] = {1, -1, 0, 0};
        const int dy[] = {0, 0, 1, -1};

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < sampler.bufferWidth &&
                ny >= 0 && ny < sampler.bufferHeight) {
                size_t idx = static_cast<size_t>(ny) *
                             static_cast<size_t>(sampler.bufferWidth) +
                             static_cast<size_t>(nx);
                if (!visited[idx]) {
                    visited[idx] = true;
                    queue.push({nx, ny});
                }
            }
        }
    }

    if (filledPixels.empty()) return;

    spriteai::core::document::Stroke fillStroke;
    fillStroke.type = spriteai::core::document::StrokeType::Fill;
    fillStroke.rgba = m_rgba;
    fillStroke.width = 1.0f;

    for (const auto& [px, py] : filledPixels) {
        fillStroke.points.push_back({
            static_cast<float>(px) + 0.5f,
            static_cast<float>(py) + 0.5f,
            1.0f
        });
    }

    doc.addStroke(fillStroke);

    auto& strokes = doc.mutableStrokes();
    const std::size_t idx = strokes.size() - 1;

    stack.execute(doc, std::make_unique<spriteai::core::command::AddStrokeCommand>(
        fillStroke, idx));
}

void FillTool::onPointerMove(spriteai::core::document::SpriteDocument&,
                             spriteai::core::command::CommandStack&,
                             const ToolInput&)
{
}

void FillTool::onPointerUp(spriteai::core::document::SpriteDocument&,
                           spriteai::core::command::CommandStack&,
                           const ToolInput&)
{
}

} // namespace spriteai::core::tools::builtin
