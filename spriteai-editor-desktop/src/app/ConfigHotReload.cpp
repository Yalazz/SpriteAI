#include "spriteai/editor/app/ConfigHotReload.h"

#include "spriteai/engine/EngineContext.h"
#include "spriteai/engine/BuiltinRegistration.h"
#include "spriteai/engine/theme/Theme.h"

#include "spriteai/core/tools/ToolRegistry.h"
#include "spriteai/core/tools/PluginLoader.h"
#include "spriteai/core/tools/builtin/BrushTool.h"
#include "spriteai/core/tools/builtin/EraserTool.h"
#include "spriteai/core/tools/builtin/AITool.h"
#include "spriteai/core/tools/builtin/PencilTool.h"
#include "spriteai/core/tools/builtin/FillTool.h"
#include "spriteai/core/tools/builtin/EyedropperTool.h"
#include "spriteai/core/tools/builtin/SelectTool.h"
#include "spriteai/core/tools/builtin/MoveTool.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QApplication>

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
    if (c.r == 0 && c.g == 0 && c.b == 0 && c.a == 0) {
        qWarning() << "Failed to parse color:" << hex;
    }
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

    // Scan for available themes
    scanAvailableThemes();

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
    const auto ui = obj.value("ui").toObject();

    t.name       = obj.value("name").toString("Default").toStdString();
    m_currentThemeName = QString::fromStdString(t.name);

    // Canvas colors
    t.background = parseHex(colors.value("background").toString("#0B0F14"));
    t.canvas     = parseHex(colors.value("canvas").toString("#0E1621"));
    t.gridMajor  = parseHex(colors.value("gridMajor").toString("#2C3A4A"));
    t.gridMinor  = parseHex(colors.value("gridMinor").toString("#1E2A38"));
    t.stroke     = parseHex(colors.value("stroke").toString("#E8F1FF"));
    t.guide      = parseHex(colors.value("guide").toString("#7FD1FF"));
    t.symmetry   = parseHex(colors.value("symmetry").toString("#FF7FDB"));

    // UI colors
    t.uiBackground    = parseHex(ui.value("background").toString("#1a1a2e"));
    t.uiSurface       = parseHex(ui.value("surface").toString("#16213e"));
    t.uiSurfaceHover  = parseHex(ui.value("surfaceHover").toString("#0f3460"));
    t.uiPrimary       = parseHex(ui.value("primary").toString("#e94560"));
    t.uiPrimaryHover  = parseHex(ui.value("primaryHover").toString("#c83250"));
    t.uiSecondary     = parseHex(ui.value("secondary").toString("#4ecdc4"));
    t.uiText          = parseHex(ui.value("text").toString("#e8e8e8"));
    t.uiTextSecondary = parseHex(ui.value("textSecondary").toString("#a0a0b4"));
    t.uiTextDisabled  = parseHex(ui.value("textDisabled").toString("#646478"));
    t.uiBorder        = parseHex(ui.value("border").toString("#0f3460"));
    t.uiSuccess       = parseHex(ui.value("success").toString("#2ecc71"));
    t.uiWarning       = parseHex(ui.value("warning").toString("#f1c40f"));
    t.uiError         = parseHex(ui.value("error").toString("#e74c3c"));

    // UI metrics
    t.borderRadius = ui.value("radius").toInt(6);
    t.spacing      = ui.value("spacing").toInt(8);
    t.fontFamily   = ui.value("font").toString("Segoe UI").toStdString();
    t.fontSize     = ui.value("fontSize").toInt(10);

    m_engine.setTheme(t);

    // Generate and apply stylesheet
    QString stylesheet = generateStylesheetFromTheme(t);
    if (qApp) {
        qApp->setStyleSheet(stylesheet);
    }
    emit stylesheetGenerated(stylesheet);
    emit themeChanged(m_currentThemeName);
}

// ------------------------------------------------------------
// Tools
// ------------------------------------------------------------

void ConfigHotReload::reloadTools() {
    QFile f(m_toolsPath);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open tools config:" << m_toolsPath;
        qWarning() << "Creating default brush tool as fallback";
        // JSON yoksa fallback
        if (auto tool = m_engine.toolRegistry().create("builtin.brush")) {
            m_engine.setActiveTool(std::move(tool));
        }
        return;
    }

    const auto doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON in tools config:" << m_toolsPath;
        qWarning() << "Creating default brush tool as fallback";
        if (auto tool = m_engine.toolRegistry().create("builtin.brush")) {
            m_engine.setActiveTool(std::move(tool));
        }
        return;
    }

    const auto obj = doc.object();
    const QString activeToolId = obj.value("activeTool").toString();

    // Shortcuts
    m_shortcuts.clear();
    const auto shortcuts = obj.value("shortcuts").toObject();
    for (auto it = shortcuts.begin(); it != shortcuts.end(); ++it)
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

    // Active tool: JSON varsa onu seç, yoksa brush fallback
    if (!activeToolId.isEmpty()) {
        if (auto tool = createToolById(activeToolId)) {
            m_engine.setActiveTool(std::move(tool));
            return;
        }
        qWarning() << "Failed to create tool:" << activeToolId;
        qWarning() << "Falling back to default brush tool";
    }

    if (auto tool = m_engine.toolRegistry().create("builtin.brush")) {
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
        if (settings.contains("spacing"))
            b->setSpacing(float(settings.value("spacing").toDouble(0.25)));
    }

    if (auto* pencil = dynamic_cast<spriteai::core::tools::builtin::PencilTool*>(tool.get())) {
        if (settings.contains("rgba"))
            pencil->setColor(parseRgbaU32(settings.value("rgba").toString()));
    }

    if (auto* fill = dynamic_cast<spriteai::core::tools::builtin::FillTool*>(tool.get())) {
        if (settings.contains("rgba"))
            fill->setColor(parseRgbaU32(settings.value("rgba").toString()));
        if (settings.contains("tolerance"))
            fill->setTolerance(settings.value("tolerance").toInt(0));
    }

    if (auto* select = dynamic_cast<spriteai::core::tools::builtin::SelectTool*>(tool.get())) {
        if (settings.contains("mode")) {
            QString mode = settings.value("mode").toString("rectangle");
            if (mode == "freeform")
                select->setMode(spriteai::core::tools::builtin::SelectMode::Freeform);
            else
                select->setMode(spriteai::core::tools::builtin::SelectMode::Rectangle);
        }
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

// ------------------------------------------------------------
// Theme Management
// ------------------------------------------------------------

void ConfigHotReload::scanAvailableThemes() {
    m_availableThemes.clear();
    QString themesDir = resolveConfigPath(m_root, "config/themes");
    QDir dir(themesDir);

    if (!dir.exists()) return;

    QStringList filters;
    filters << "*.theme.json";
    dir.setNameFilters(filters);

    for (const QString& file : dir.entryList(QDir::Files)) {
        QString themeName = file;
        themeName.remove(".theme.json");
        m_availableThemes.append(themeName);
    }
}

QStringList ConfigHotReload::availableThemes() const {
    return m_availableThemes;
}

QString ConfigHotReload::currentThemeName() const {
    return m_currentThemeName;
}

void ConfigHotReload::setTheme(const QString& themeName) {
    QString newPath = resolveConfigPath(m_root, QString("config/themes/%1.theme.json").arg(themeName));
    if (QFile::exists(newPath)) {
        m_themePath = newPath;
        if (!m_watcher.files().contains(newPath)) {
            m_watcher.addPath(newPath);
        }
        reloadTheme();
    }
}

QString ConfigHotReload::generateStylesheet() const {
    return generateStylesheetFromTheme(m_engine.theme());
}

QString ConfigHotReload::generateStylesheetFromTheme(const spriteai::engine::Theme& t) const {
    QString bg = QString::fromStdString(t.uiBackground.toHex());
    QString surface = QString::fromStdString(t.uiSurface.toHex());
    QString surfaceHover = QString::fromStdString(t.uiSurfaceHover.toHex());
    QString primary = QString::fromStdString(t.uiPrimary.toHex());
    QString primaryHover = QString::fromStdString(t.uiPrimaryHover.toHex());
    QString secondary = QString::fromStdString(t.uiSecondary.toHex());
    QString text = QString::fromStdString(t.uiText.toHex());
    QString textSecondary = QString::fromStdString(t.uiTextSecondary.toHex());
    QString textDisabled = QString::fromStdString(t.uiTextDisabled.toHex());
    QString border = QString::fromStdString(t.uiBorder.toHex());
    QString success = QString::fromStdString(t.uiSuccess.toHex());
    QString warning = QString::fromStdString(t.uiWarning.toHex());
    QString error = QString::fromStdString(t.uiError.toHex());
    int radius = t.borderRadius;
    QString font = QString::fromStdString(t.fontFamily);
    int fontSize = t.fontSize;

    return QString(R"(
/* Dynamic Theme: %1 */

* {
    font-family: "%2";
    font-size: %3pt;
}

QMainWindow {
    background-color: %4;
}

QMenuBar {
    background-color: %5;
    color: %6;
    border-bottom: 1px solid %7;
    padding: 4px;
}

QMenuBar::item {
    background-color: transparent;
    padding: 6px 12px;
    border-radius: %8px;
}

QMenuBar::item:selected {
    background-color: %9;
}

QMenuBar::item:pressed {
    background-color: %10;
}

QMenu {
    background-color: %5;
    color: %6;
    border: 1px solid %7;
    border-radius: %8px;
    padding: 4px;
}

QMenu::item {
    padding: 8px 24px;
    border-radius: %8px;
}

QMenu::item:selected {
    background-color: %9;
}

QMenu::separator {
    height: 1px;
    background-color: %7;
    margin: 4px 8px;
}

QToolBar {
    background-color: %5;
    border: none;
    spacing: 4px;
    padding: 4px;
}

QToolButton {
    background-color: transparent;
    border: none;
    border-radius: %8px;
    padding: 6px;
    color: %6;
}

QToolButton:hover {
    background-color: %9;
}

QToolButton:pressed, QToolButton:checked {
    background-color: %10;
}

QDockWidget {
    color: %6;
    titlebar-close-icon: none;
    titlebar-normal-icon: none;
}

QDockWidget::title {
    background-color: %5;
    padding: 8px;
    border-bottom: 1px solid %7;
}

QDockWidget::close-button, QDockWidget::float-button {
    background-color: transparent;
    border: none;
    padding: 2px;
}

QDockWidget::close-button:hover, QDockWidget::float-button:hover {
    background-color: %9;
    border-radius: %8px;
}

QWidget {
    background-color: %4;
    color: %6;
}

QScrollArea {
    border: none;
    background-color: transparent;
}

QScrollBar:vertical {
    background-color: %5;
    width: 12px;
    margin: 0;
    border-radius: 6px;
}

QScrollBar::handle:vertical {
    background-color: %9;
    min-height: 30px;
    border-radius: 6px;
    margin: 2px;
}

QScrollBar::handle:vertical:hover {
    background-color: %10;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0;
}

QScrollBar:horizontal {
    background-color: %5;
    height: 12px;
    margin: 0;
    border-radius: 6px;
}

QScrollBar::handle:horizontal {
    background-color: %9;
    min-width: 30px;
    border-radius: 6px;
    margin: 2px;
}

QPushButton {
    background-color: %10;
    color: %6;
    border: 1px solid %7;
    border-radius: %8px;
    padding: 8px 16px;
}

QPushButton:hover {
    background-color: %9;
}

QPushButton:pressed {
    background-color: %5;
}

QPushButton:disabled {
    background-color: %5;
    color: %11;
}

QLineEdit, QSpinBox, QDoubleSpinBox {
    background-color: %5;
    color: %6;
    border: 1px solid %7;
    border-radius: %8px;
    padding: 6px;
}

QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {
    border-color: %10;
}

QComboBox {
    background-color: %5;
    color: %6;
    border: 1px solid %7;
    border-radius: %8px;
    padding: 6px 12px;
}

QComboBox::drop-down {
    border: none;
    width: 24px;
}

QComboBox QAbstractItemView {
    background-color: %5;
    color: %6;
    border: 1px solid %7;
    border-radius: %8px;
    selection-background-color: %9;
}

QSlider::groove:horizontal {
    background-color: %5;
    height: 6px;
    border-radius: 3px;
}

QSlider::handle:horizontal {
    background-color: %10;
    width: 16px;
    height: 16px;
    margin: -5px 0;
    border-radius: 8px;
}

QSlider::handle:horizontal:hover {
    background-color: %12;
}

QListWidget {
    background-color: %5;
    border: 1px solid %7;
    border-radius: %8px;
}

QListWidget::item {
    padding: 8px;
    border-radius: %8px;
}

QListWidget::item:selected {
    background-color: %9;
}

QListWidget::item:hover {
    background-color: %9;
}

QLabel {
    background-color: transparent;
    color: %6;
}

QStatusBar {
    background-color: %5;
    color: %13;
    border-top: 1px solid %7;
}

QGroupBox {
    border: 1px solid %7;
    border-radius: %8px;
    margin-top: 12px;
    padding-top: 8px;
}

QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    padding: 0 6px;
    color: %6;
}

QCheckBox::indicator {
    width: 18px;
    height: 18px;
    border-radius: 4px;
    border: 1px solid %7;
    background-color: %5;
}

QCheckBox::indicator:checked {
    background-color: %10;
    border-color: %10;
}

QRadioButton::indicator {
    width: 18px;
    height: 18px;
    border-radius: 9px;
    border: 1px solid %7;
    background-color: %5;
}

QRadioButton::indicator:checked {
    background-color: %10;
    border-color: %10;
}

QTabWidget::pane {
    border: 1px solid %7;
    border-radius: %8px;
    background-color: %5;
}

QTabBar::tab {
    background-color: %5;
    color: %6;
    padding: 8px 16px;
    border-top-left-radius: %8px;
    border-top-right-radius: %8px;
}

QTabBar::tab:selected {
    background-color: %9;
}

QTabBar::tab:hover:!selected {
    background-color: %9;
}
)")
    .arg(QString::fromStdString(t.name))  // %1
    .arg(font)                              // %2
    .arg(fontSize)                          // %3
    .arg(bg)                                // %4
    .arg(surface)                           // %5
    .arg(text)                              // %6
    .arg(border)                            // %7
    .arg(radius)                            // %8
    .arg(surfaceHover)                      // %9
    .arg(primary)                           // %10
    .arg(textDisabled)                      // %11
    .arg(primaryHover)                      // %12
    .arg(textSecondary);                    // %13
}

} // namespace spriteai::editor::app
