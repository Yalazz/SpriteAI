#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace spriteai::core::tools {

class Tool;

using ToolFactoryFn = std::function<std::unique_ptr<Tool>()>;

class ToolRegistry {
public:
    bool registerFactory(const std::string& type, ToolFactoryFn fn);
    std::unique_ptr<Tool> create(const std::string& type) const;

    std::vector<std::string> types() const;

private:
    std::unordered_map<std::string, ToolFactoryFn> m_factories;
};

} // namespace spriteai::core::tools
