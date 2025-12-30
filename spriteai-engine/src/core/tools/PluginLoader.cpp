#include "spriteai/core/tools/PluginLoader.h"
#include "spriteai/core/tools/ToolRegistry.h"
#include "spriteai/core/tools/ToolPlugin.h"

#include <filesystem>

#if defined(_WIN32)
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif

namespace fs = std::filesystem;

namespace spriteai::core::tools {

static bool looks_like_shared_lib(const fs::path& p) {
#if defined(_WIN32)
    return p.extension() == ".dll";
#elif defined(__APPLE__)
    return p.extension() == ".dylib";
#else
    return p.extension() == ".so";
#endif
}

bool PluginLoader::loadPlugin(const std::string& sharedLibPath, ToolRegistry& registry) {
#if defined(_WIN32)
    HMODULE lib = LoadLibraryA(sharedLibPath.c_str());
    if (!lib) return false;
    auto fn = reinterpret_cast<SpriteAI_RegisterToolsFn>(GetProcAddress(lib, "spriteai_register_tools"));
    if (!fn) return false;
    fn(registry);
    return true;
#else
    void* lib = dlopen(sharedLibPath.c_str(), RTLD_NOW);
    if (!lib) return false;
    auto fn = reinterpret_cast<SpriteAI_RegisterToolsFn>(dlsym(lib, "spriteai_register_tools"));
    if (!fn) return false;
    fn(registry);
    return true;
#endif
}

int PluginLoader::loadAllFromFolder(const std::string& folderPath, ToolRegistry& registry) {
    int loaded = 0;
    std::error_code ec;
    for (const auto& e : fs::directory_iterator(folderPath, ec)) {
        if (ec) break;
        if (!e.is_regular_file()) continue;
        const auto& p = e.path();
        if (!looks_like_shared_lib(p)) continue;
        loaded += loadPlugin(p.string(), registry) ? 1 : 0;
    }
    return loaded;
}

} // namespace spriteai::core::tools
