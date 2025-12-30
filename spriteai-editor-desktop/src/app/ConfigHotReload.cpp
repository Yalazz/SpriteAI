#include "spriteai/editor/app/ConfigHotReload.h"

#include "spriteai/engine/EngineContext.h"
#include "spriteai/engine/BuiltinRegistration.h"

#include "spriteai/core/tools/ToolRegistry.h"
#include "spriteai/core/tools/PluginLoader.h"
#include "spriteai/core/tools/builtin/BrushTool.h"
#include "spriteai/core/tools/builtin/EraserTool.h"
#include "spriteai/core/tools/builtin/AITool.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace spriteai::editor::app {

// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------

static spriteai::engine::ColorRGBA8 parseHex(const QString& hex) {
    QString h = hex.trimmed();
    if (h.startsWith("#")) h.remove(0, 1);

    bool ok = false;
    const auto val = h.toUInt(&ok, 16);
    if (!ok) return {};

    spriteai::engine::ColorRGBA8 c{};
    if (h.size() == 6) {
        c.r = (val >> 16) & 0xFF;
        c.g = (val >> 8) & 0xFF;
        c.b = val & 0xFF;
        c.a = 255;
    } else if (h.size() == 8) {
        c.r = (val >> 24) & 0xFF;
        c.g = (val >> 16) & 0xFF;
        c.b = (val >> 8) & 0xFF;
        c.a = val & 0xFF;
    }
    return c;
}

    static std::uint32_t parseRgbaU32(const QString& hex) {
    const auto c = parseHex(hex);
    return (std::uint32_t(c.a) << 24)  // A
         | (std::uint32_t(c.r) << 16)  // R
         | (std::uint32_t(c.g) << 8)   // G
         |  std::uint32_t(c.b);        // B
}


// ------------------------------------------------------------
// Ctor
// ------------------------------------------------------------

ConfigHotReload::ConfigHotReload(spriteai::engine::EngineContext& engine, QObject* parent)
    : QObject(parent)
    , m_engine(engine)
{
    connect(&m_watcher, &QFileSystemWatcher::fileChanged,
            this, &ConfigHotReload::onFileChanged);

    spriteai::engine::registerBuiltinTools(m_engine.toolRegistry());
}

// ------------------------------------------------------------

void ConfigHotReload::setProjectRoot(const QString& rootPath) {
    m_root = rootPath;
}

QString ConfigHotReload::resolveConfigPath(const QString& root, const QString& rel) {
    return root.isEmpty() ? QDir::current().filePath(rel)
                          : QDir(root).filePath(rel);
}

// ------------------------------------------------------------

void ConfigHotReload::start() {
    m_themePath = resolveConfigPath(m_root, "config/themes/default.theme.json");
    m_toolsPath = resolveConfigPath(m_root, "config/tools/default.tools.json");

    if (QFile::exists(m_themePath)) m_watcher.addPath(m_themePath);
    if (QFile::exists(m_toolsPath)) m_watcher.addPath(m_toolsPath);

    const QString pluginsDir = resolveConfigPath(m_root, "plugins");
    spriteai::core::tools::PluginLoader::loadAllFromFolder(
        pluginsDir.toStdString(),
        m_engine.toolRegistry()
    );

    reloadAll();
}

// ------------------------------------------------------------

void ConfigHotReload::reloadAll() {
    reloadTheme();
    reloadTools();
    emit configReloaded();
}

void ConfigHotReload::onFileChanged(const QString& path) {
    if (QFile::exists(path))
        m_watcher.addPath(path);

    if (path == m_themePath) reloadTheme();
    if (path == m_toolsPath) reloadTools();

    emit configReloaded();
}

// ------------------------------------------------------------
// Theme
// ------------------------------------------------------------

void ConfigHotReload::reloadTheme() {
    QFile f(m_themePath);
    if (!f.open(QIODevice::ReadOnly)) return;

    const auto doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isObject()) return;

    spriteai::engine::Theme t;
    const auto obj = doc.object();
    const auto colors = obj.value("colors").toObject();

    t.name       = obj.value("name").toString("Default").toStdString();
    t.background = parseHex(colors.value("background").toString("#0B0F14"));
    t.canvas     = parseHex(colors.value("canvas").toString("#0E1621"));
    t.gridMajor  = parseHex(colors.value("gridMajor").toString("#2C3A4A"));
    t.gridMinor  = parseHex(colors.value("gridMinor").toString("#1E2A38"));
    t.stroke     = parseHex(colors.value("stroke").toString("#E8F1FF"));
    t.guide      = parseHex(colors.value("guide").toString("#7FD1FF"));
    t.symmetry   = parseHex(colors.value("symmetry").toString("#FF7FDB"));

    m_engine.setTheme(t);
}

// ------------------------------------------------------------
// Tools
// ------------------------------------------------------------

void ConfigHotReload::reloadTools() {
    QFile f(m_toolsPath);
    if (!f.open(QIODevice::ReadOnly)) return;

    const auto doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isObject()) return;

    const auto obj = doc.object();
    const QString activeToolId = obj.value("activeTool").toString();

    // Shortcuts
    m_shortcuts.clear();
    for (auto it = obj.value("shortcuts").toObject().begin();
         it != obj.value("shortcuts").toObject().end(); ++it)
    {
        const QString key = it.key().toUpper();
        if (key.size() == 1)
            m_shortcuts.insert(key[0].unicode(), it.value().toString());
    }

    // Tool defs
    m_toolTypes.clear();
    m_toolSettings.clear();

    for (const auto& v : obj.value("tools").toArray()) {
        if (!v.isObject()) continue;
        const auto o = v.toObject();

        const QString id   = o.value("id").toString();
        const QString type = o.value("type").toString();

        if (!id.isEmpty() && !type.isEmpty()) {
            m_toolTypes.insert(id, type);
            m_toolSettings.insert(id, o.value("settings").toObject());
        }
    }

    if (!activeToolId.isEmpty()) {
        if (auto tool = createToolById(activeToolId))
            m_engine.setActiveTool(std::move(tool));
    }
}

// ------------------------------------------------------------
// Tool factory
// ------------------------------------------------------------

std::unique_ptr<spriteai::core::tools::Tool>
ConfigHotReload::createToolById(const QString& toolId) const
{
    const auto type = m_toolTypes.value(toolId);
    if (type.isEmpty()) return {};

    auto tool = m_engine.toolRegistry().create(type.toStdString());
    if (!tool) return {};

    const auto settings = m_toolSettings.value(toolId);

    if (auto* b = dynamic_cast<spriteai::core::tools::builtin::BrushTool*>(tool.get())) {
        if (settings.contains("size"))
            b->setWidth(float(settings.value("size").toDouble(6.0)));
        if (settings.contains("rgba"))
            b->setColor(parseRgbaU32(settings.value("rgba").toString()));
    }

    if (auto* ai = dynamic_cast<spriteai::core::tools::builtin::AITool*>(tool.get())) {
        if (settings.contains("mode"))
            ai->setMode(settings.value("mode").toString("clean").toStdString());
    }

    return tool;
}

// ------------------------------------------------------------

QString ConfigHotReload::toolIdForShortcut(int qtKey) const {
    return m_shortcuts.value(qtKey);
}

} // namespace spriteai::editor::app
