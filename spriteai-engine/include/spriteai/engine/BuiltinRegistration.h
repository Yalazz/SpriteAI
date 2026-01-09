#pragma once
#include "spriteai/core/tools/ToolRegistry.h"
#include "spriteai/core/tools/builtin/BrushTool.h"
#include "spriteai/core/tools/builtin/EraserTool.h"
#include "spriteai/core/tools/builtin/AITool.h"
#include "spriteai/core/tools/builtin/PencilTool.h"
#include "spriteai/core/tools/builtin/FillTool.h"
#include "spriteai/core/tools/builtin/EyedropperTool.h"
#include "spriteai/core/tools/builtin/SelectTool.h"
#include "spriteai/core/tools/builtin/MoveTool.h"

namespace spriteai::engine {

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
    registry.registerFactory("builtin.pencil", []{
        return std::make_unique<spriteai::core::tools::builtin::PencilTool>();
    });
    registry.registerFactory("builtin.fill", []{
        return std::make_unique<spriteai::core::tools::builtin::FillTool>();
    });
    registry.registerFactory("builtin.eyedropper", []{
        return std::make_unique<spriteai::core::tools::builtin::EyedropperTool>();
    });
    registry.registerFactory("builtin.select", []{
        return std::make_unique<spriteai::core::tools::builtin::SelectTool>();
    });
    registry.registerFactory("builtin.move", []{
        return std::make_unique<spriteai::core::tools::builtin::MoveTool>();
    });
}

} // namespace spriteai::engine
