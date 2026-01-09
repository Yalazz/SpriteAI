#include "spriteai/engine/EngineContext.h"

#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/core/command/CommandStack.h"
#include "spriteai/core/tools/ToolRegistry.h"
#include "spriteai/core/tools/Tool.h"
#include "spriteai/core/tools/ToolContext.h"
#include "spriteai/core/selection/Selection.h"
#include "spriteai/engine/ai/IAIClient.h"

namespace spriteai::engine {

using spriteai::core::tools::ToolInput;

struct EngineContext::Impl {
    core::document::SpriteDocument doc;
    core::command::CommandStack    cmd;
    core::tools::ToolRegistry      registry;
    core::tools::ToolContext       toolContext;
    core::selection::Selection     selection;

    std::unique_ptr<core::tools::Tool> activeTool;

    Theme theme{};
    overlays::GridSettings     grid{};
    overlays::SymmetrySettings symmetry{};
    overlays::GuideSettings    guides{};

    std::shared_ptr<ai::IAIClient> aiClient{};
};

EngineContext::EngineContext()
    : m(std::make_unique<Impl>())
{
    m->theme.canvas = ColorRGBA8{255,255,255,255};
    m->toolContext.setSelection(&m->selection);
    m->toolContext.setSymmetry(&m->symmetry);
}

EngineContext::~EngineContext() = default;

core::document::SpriteDocument& EngineContext::document() { return m->doc; }
core::command::CommandStack& EngineContext::commandStack() { return m->cmd; }

void EngineContext::undo() { m->cmd.undo(m->doc); }
void EngineContext::redo() { m->cmd.redo(m->doc); }

core::tools::ToolRegistry& EngineContext::toolRegistry() { return m->registry; }

void EngineContext::setActiveTool(std::unique_ptr<core::tools::Tool> tool) {
    if (tool) {
        tool->setToolContext(&m->toolContext);
    }
    m->activeTool = std::move(tool);
}

core::tools::Tool* EngineContext::activeTool() const {
    return m->activeTool.get();
}

core::tools::ToolContext& EngineContext::toolContext() { return m->toolContext; }
const core::tools::ToolContext& EngineContext::toolContext() const { return m->toolContext; }

core::selection::Selection& EngineContext::selection() { return m->selection; }
const core::selection::Selection& EngineContext::selection() const { return m->selection; }

static ToolInput makeInput(float x, float y, float pressure,
                           bool alt, bool shift, bool down)
{
    return ToolInput{
        .x = x,
        .y = y,
        .pressure = pressure,
        .primaryDown = down,
        .alt = alt,
        .shift = shift
    };
}

void EngineContext::pointerDown(float x, float y, float pressure,
                                bool alt, bool shift)
{
    if (!m->activeTool) return;
    m->activeTool->onPointerDown(m->doc, m->cmd, makeInput(x,y,pressure,alt,shift,true));
}

void EngineContext::pointerMove(float x, float y, float pressure,
                                bool alt, bool shift)
{
    if (!m->activeTool) return;
    m->activeTool->onPointerMove(m->doc, m->cmd, makeInput(x,y,pressure,alt,shift,true));
}

void EngineContext::pointerUp(float x, float y, float pressure,
                              bool alt, bool shift)
{
    if (!m->activeTool) return;
    m->activeTool->onPointerUp(m->doc, m->cmd, makeInput(x,y,pressure,alt,shift,false));
}

void EngineContext::setTheme(const Theme& t) { m->theme = t; }
const Theme& EngineContext::theme() const { return m->theme; }

overlays::GridSettings& EngineContext::grid() { return m->grid; }
const overlays::GridSettings& EngineContext::grid() const { return m->grid; }

overlays::SymmetrySettings& EngineContext::symmetry() { return m->symmetry; }
const overlays::SymmetrySettings& EngineContext::symmetry() const { return m->symmetry; }

overlays::GuideSettings& EngineContext::guides() { return m->guides; }
const overlays::GuideSettings& EngineContext::guides() const { return m->guides; }

void EngineContext::setAIClient(std::shared_ptr<ai::IAIClient> client) {
    m->aiClient = std::move(client);
}
std::shared_ptr<ai::IAIClient> EngineContext::aiClient() const { return m->aiClient; }

} // namespace spriteai::engine
