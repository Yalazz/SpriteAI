#pragma once

#include <QObject>     // 🔥 KRİTİK
#include <QWidget>
#include <memory>

// Forward declarations (UI layer only sees interfaces)
namespace spriteai::engine { class EngineContext; }
namespace spriteai::canvas { class CanvasView; }
namespace spriteai::engine::renderer { class ICanvasRenderer; }

namespace spriteai::editor::app {

    class ConfigHotReload;

    class CanvasWidget final : public QWidget {
        Q_OBJECT

    public:
        explicit CanvasWidget(QWidget* parent = nullptr);
        ~CanvasWidget() override;

    protected:
        void paintEvent(QPaintEvent*) override;
        void mousePressEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
        void mouseReleaseEvent(QMouseEvent*) override;
        void wheelEvent(QWheelEvent*) override;
        void keyPressEvent(QKeyEvent*) override;

    private:
        std::unique_ptr<spriteai::engine::EngineContext> m_engine;
        std::unique_ptr<spriteai::canvas::CanvasView> m_view;
        std::unique_ptr<spriteai::engine::renderer::ICanvasRenderer> m_renderer;
        std::unique_ptr<ConfigHotReload> m_hotReload;

        bool m_updateScheduled = false;
        void sendPointer(bool down, QMouseEvent* e);
        void scheduleUpdate();
    };

} // namespace spriteai::editor::app
