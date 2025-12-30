#include "spriteai/core/tools/ToolRegistry.h"
#include "spriteai/core/tools/ToolPlugin.h"
#include "spriteai/core/tools/builtin/BrushTool.h"

extern "C" void spriteai_register_tools(spriteai::core::tools::ToolRegistry& registry) {
    registry.registerFactory("plugin.brush_plus", []{
        auto t = std::make_unique<spriteai::core::tools::builtin::BrushTool>(0xFFAAFFFFu, 14.0f);
        return t;
    });
}
