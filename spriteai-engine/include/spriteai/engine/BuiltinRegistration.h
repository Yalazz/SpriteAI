#pragma once
#include "spriteai/core/tools/ToolRegistry.h"
#include "spriteai/core/tools/builtin/BrushTool.h"
#include "spriteai/core/tools/builtin/EraserTool.h"
#include "spriteai/core/tools/builtin/AITool.h"

namespace spriteai::engine {

// Registers built-in tool factories.
// Call once during app startup.
inline void registerBuiltinTools(spriteai::core::tools::ToolRegistry& registry) {
    registry.registerFactory("builtin.brush", []{
        return std::make_unique<spriteai::core::tools::builtin::BrushTool>();
    });
    registry.registerFactory("builtin.eraser", []{
        return std::make_unique<spriteai::core::tools::builtin::EraserTool>();
    });
    registry.registerFactory("builtin.ai", []{
        return std::make_unique<spriteai::core::tools::builtin::AITool>();
    });
}

} // namespace spriteai::engine
