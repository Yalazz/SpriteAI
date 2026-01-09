#include "spriteai/core/tools/builtin/EyedropperTool.h"
#include "spriteai/core/tools/ToolContext.h"
#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/core/command/CommandStack.h"

namespace spriteai::core::tools::builtin {

void EyedropperTool::pickColor(const ToolInput& in) {
    if (!m_toolContext) return;

    const auto& sampler = m_toolContext->pixelSampler();
    if (!sampler.sampleAt) return;

    auto colorOpt = sampler.sampleAt(in.x, in.y);
    if (!colorOpt) return;

    m_lastColor = *colorOpt;

    const auto& callback = m_toolContext->colorCallback();
    if (callback.onColorPicked) {
        callback.onColorPicked(m_lastColor);
    }

    m_toolContext->setForegroundColor(m_lastColor);
}

void EyedropperTool::onPointerDown(spriteai::core::document::SpriteDocument&,
                                   spriteai::core::command::CommandStack&,
                                   const ToolInput& in)
{
    m_picking = true;
    pickColor(in);
}

void EyedropperTool::onPointerMove(spriteai::core::document::SpriteDocument&,
                                   spriteai::core::command::CommandStack&,
                                   const ToolInput& in)
{
    if (!m_picking) return;
    pickColor(in);
}

void EyedropperTool::onPointerUp(spriteai::core::document::SpriteDocument&,
                                 spriteai::core::command::CommandStack&,
                                 const ToolInput&)
{
    m_picking = false;
}

} // namespace spriteai::core::tools::builtin
