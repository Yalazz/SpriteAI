#pragma once
#include <cstdint>

namespace spriteai::core::tools { class ToolRegistry; }

// Plugin ABI: a shared library exports `spriteai_register_tools`.
extern "C" {
    using SpriteAI_RegisterToolsFn = void(*)(spriteai::core::tools::ToolRegistry&);
}
