#include "spriteai/core/tools/ToolRegistry.h"
#include "spriteai/core/tools/Tool.h"

namespace spriteai::core::tools {

bool ToolRegistry::registerFactory(const std::string& type, ToolFactoryFn fn) {
    if (type.empty() || !fn) return false;
    return m_factories.emplace(type, std::move(fn)).second;
}

std::unique_ptr<Tool> ToolRegistry::create(const std::string& type) const {
    auto it = m_factories.find(type);
    if (it == m_factories.end()) return {};
    return (it->second)();
}

std::vector<std::string> ToolRegistry::types() const {
    std::vector<std::string> out;
    out.reserve(m_factories.size());
    for (const auto& kv : m_factories) out.push_back(kv.first);
    return out;
}

} // namespace spriteai::core::tools
