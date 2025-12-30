#pragma once
#include <string>
#include <vector>

namespace spriteai::core::tools {

class ToolRegistry;

class PluginLoader {
public:
    // Loads one plugin and calls its register function if found.
    // Returns true if registration succeeded.
    static bool loadPlugin(const std::string& sharedLibPath, ToolRegistry& registry);

    // Convenience: loads all files in a folder that look like shared libs.
    static int loadAllFromFolder(const std::string& folderPath, ToolRegistry& registry);
};

} // namespace spriteai::core::tools
