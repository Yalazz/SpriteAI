#pragma once

#include <QObject>
#include <QWidget>
#include <QColor>
#include <memory>
#include <cstdint>

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

        // Tool management
        void setTool(const QString& toolId);

        // Edit operations
        void undo();
        void redo();

        // View operations
        void zoomIn();
        void zoomOut();
        void zoomFit();
        void setZoom(float zoom);
        float zoom() const;

        // Color operations
        void setForegroundColor(const QColor& color);
        void setBackgroundColor(const QColor& color);

        // Brush settings
        void setBrushSize(int size);
        void setBrushOpacity(int opacity);
        void setBrushSpacing(int spacing);

        // Grid and overlays
        void setGridEnabled(bool enabled);
        void setSymmetryEnabled(bool enabled);

        // Edit operations
        void selectAll();
        void deselectAll();
        void deleteSelection();

        // Image operations
        void flipHorizontal();
        void flipVertical();
        void rotate90CW();
        void rotate90CCW();
        void clearCanvas();

        // Layer operations
        int addLayer();
        void deleteLayer(int index);
        void duplicateLayer(int index);
        void moveLayer(int from, int to);
        void setActiveLayer(int index);
        void setLayerOpacity(int index, int opacity);
        void setLayerBlendMode(int index, const QString& mode);
        int layerCount() const;
        int activeLayerIndex() const;

    signals:
        void toolChanged(const QString& toolId);
        void zoomChanged(float zoom);
        void colorPicked(std::uint32_t rgba);
        void layerCountChanged(int count);
        void activeLayerChanged(int index);

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
        void setupPixelSampler();
    };

} // namespace spriteai::editor::app
