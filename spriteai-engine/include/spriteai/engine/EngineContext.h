#pragma once

#include <memory>

#include "spriteai/engine/theme/Theme.h"
#include "spriteai/engine/overlays/OverlaySettings.h"
#include "spriteai/core/tools/Tool.h"

namespace spriteai::core::document { class SpriteDocument; }
namespace spriteai::core::command { class CommandStack; }
namespace spriteai::core::tools { class ToolRegistry; }
namespace spriteai::engine::ai { class IAIClient; }

namespace spriteai::engine {

class EngineContext {
public:
    EngineContext();
    ~EngineContext();

    // ---------------- Document ----------------
    spriteai::core::document::SpriteDocument& document();

    // ---------------- Undo / Redo ----------------
    void undo();
    void redo();

    // ---------------- Tools ----------------
    spriteai::core::tools::ToolRegistry& toolRegistry();
    void setActiveTool(std::unique_ptr<spriteai::core::tools::Tool> tool);
    spriteai::core::tools::Tool* activeTool() const;

    // ---------------- Pointer Input ----------------
    void pointerDown(float x, float y, float pressure, bool alt, bool shift);
    void pointerMove(float x, float y, float pressure, bool alt, bool shift);
    void pointerUp  (float x, float y, float pressure, bool alt, bool shift);

    // ---------------- Theme / overlays ----------------
    void setTheme(const Theme& t);
    const Theme& theme() const;

    overlays::GridSettings& grid();
    const overlays::GridSettings& grid() const;

    overlays::SymmetrySettings& symmetry();
    const overlays::SymmetrySettings& symmetry() const;

    overlays::GuideSettings& guides();
    const overlays::GuideSettings& guides() const;

    // ---------------- AI ----------------
    void setAIClient(std::shared_ptr<spriteai::engine::ai::IAIClient> client);
    std::shared_ptr<spriteai::engine::ai::IAIClient> aiClient() const;

    // ---------------- Internal ----------------
    spriteai::core::command::CommandStack& commandStack();

private:
    struct Impl;
    std::unique_ptr<Impl> m;
};

} // namespace spriteai::engine
