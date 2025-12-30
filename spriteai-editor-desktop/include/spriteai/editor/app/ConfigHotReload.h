#pragma once

#include <QObject>
#include <QFileSystemWatcher>
#include <QHash>
#include <QJsonObject>
#include <QString>
#include <memory>

namespace spriteai::engine { class EngineContext; }
namespace spriteai::core::tools { class Tool; }

namespace spriteai::editor::app {

    // Watches JSON files and pushes updates into EngineContext.
    // - theme JSON -> engine.setTheme()
    // - tools JSON -> engine.toolRegistry().create(...) and engine.setActiveTool(...)
    class ConfigHotReload final : public QObject {
        Q_OBJECT
    public:
        explicit ConfigHotReload(spriteai::engine::EngineContext& engine, QObject* parent = nullptr);

        // Root folder that contains /config. If empty, uses current working dir.
        void setProjectRoot(const QString& rootPath);

        // Start watching and apply initial config.
        void start();

        // Manual reload if needed
        void reloadAll();

        // For CanvasWidget key handling
        QString toolIdForShortcut(int qtKey) const;

        // Creates a tool from the last loaded tools.json (and applies settings).
        std::unique_ptr<spriteai::core::tools::Tool> createToolById(const QString& toolId) const;

        signals:
            void configReloaded();

    private slots:
        void onFileChanged(const QString& path);

    private:
        static QString resolveConfigPath(const QString& root, const QString& rel);

        void reloadTheme();
        void reloadTools();

    private:
        spriteai::engine::EngineContext& m_engine;
        QFileSystemWatcher m_watcher;

        QString m_root;
        QString m_themePath;
        QString m_toolsPath;

        QHash<int, QString> m_shortcuts;                 // Qt::Key -> toolId
        QHash<QString, QString> m_toolTypes;             // toolId -> registry type
        QHash<QString, QJsonObject> m_toolSettings;      // toolId -> settings
    };

} // namespace spriteai::editor::app
