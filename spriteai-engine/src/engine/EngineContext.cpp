#include "spriteai/engine/EngineContext.h"

#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/core/command/CommandStack.h"
#include "spriteai/core/tools/ToolRegistry.h"
#include "spriteai/core/tools/Tool.h"            // 🔴 ZORUNLU
#include "spriteai/engine/ai/IAIClient.h"

namespace spriteai::engine {

using spriteai::core::tools::ToolInput;

struct EngineContext::Impl {
    core::document::SpriteDocument doc;
    core::command::CommandStack    cmd;
    core::tools::ToolRegistry      registry;

    std::unique_ptr<core::tools::Tool> activeTool;

    Theme theme{};
    overlays::GridSettings     grid{};
    overlays::SymmetrySettings symmetry{};
    overlays::GuideSettings    guides{};

    std::shared_ptr<ai::IAIClient> aiClient{};
};

// ------------------------------------------------------------

EngineContext::EngineContext()
    : m(std::make_unique<Impl>()) {}

EngineContext::~EngineContext() = default;

// ------------------------------------------------------------
// Document
// ------------------------------------------------------------

core::document::SpriteDocument& EngineContext::document() {
    return m->doc;
}

// ------------------------------------------------------------
// Command stack
// ------------------------------------------------------------

core::command::CommandStack& EngineContext::commandStack() {
    return m->cmd;
}

// ------------------------------------------------------------
// Undo / Redo
// ------------------------------------------------------------

void EngineContext::undo() {
    m->cmd.undo(m->doc);
}

void EngineContext::redo() {
    m->cmd.redo(m->doc);
}

// ------------------------------------------------------------
// Tools
// ------------------------------------------------------------

core::tools::ToolRegistry& EngineContext::toolRegistry() {
    return m->registry;
}

void EngineContext::setActiveTool(std::unique_ptr<core::tools::Tool> tool) {
    m->activeTool = std::move(tool);
}

core::tools::Tool* EngineContext::activeTool() const {
    return m->activeTool.get();
}

// ------------------------------------------------------------
// Pointer adapter
// ------------------------------------------------------------

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
    m->activeTool->onPointerDown(
        m->doc, m->cmd, makeInput(x, y, pressure, alt, shift, true));
}

void EngineContext::pointerMove(float x, float y, float pressure,
                                bool alt, bool shift)
{
    if (!m->activeTool) return;
    m->activeTool->onPointerMove(
        m->doc, m->cmd, makeInput(x, y, pressure, alt, shift, true));
}

void EngineContext::pointerUp(float x, float y, float pressure,
                              bool alt, bool shift)
{
    if (!m->activeTool) return;
    m->activeTool->onPointerUp(
        m->doc, m->cmd, makeInput(x, y, pressure, alt, shift, false));
}

// ------------------------------------------------------------
// Theme / overlays
// ------------------------------------------------------------

void EngineContext::setTheme(const Theme& t) {
    m->theme = t;
}

const Theme& EngineContext::theme() const {
    return m->theme;
}

overlays::GridSettings& EngineContext::grid() { return m->grid; }
const overlays::GridSettings& EngineContext::grid() const { return m->grid; }

overlays::SymmetrySettings& EngineContext::symmetry() { return m->symmetry; }
const overlays::SymmetrySettings& EngineContext::symmetry() const { return m->symmetry; }

overlays::GuideSettings& EngineContext::guides() { return m->guides; }
const overlays::GuideSettings& EngineContext::guides() const { return m->guides; }

// ------------------------------------------------------------
// AI
// ------------------------------------------------------------

void EngineContext::setAIClient(std::shared_ptr<ai::IAIClient> client) {
    m->aiClient = std::move(client);
}

std::shared_ptr<ai::IAIClient> EngineContext::aiClient() const {
    return m->aiClient;
}

} // namespace spriteai::engine
