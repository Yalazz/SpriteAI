#pragma once

#include <QObject>
#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace spriteai::engine { class EngineContext; }

namespace spriteai::editor::app {

class CanvasWidget;
class ToolsDockWidget;
class ColorDockWidget;
class BrushSettingsDockWidget;
class LayersDockWidget;
class HistoryDockWidget;

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    CanvasWidget* canvasWidget() const { return m_canvasWidget; }

private slots:
    // File actions
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onSaveFileAs();
    void onExportFile();

    // Edit actions
    void onUndo();
    void onRedo();

    // View actions
    void onZoomIn();
    void onZoomOut();
    void onZoomFit();
    void onZoom100();
    void onToggleGrid(bool checked);
    void onToggleSymmetry(bool checked);

    // Tool actions
    void onToolBrush();
    void onToolEraser();
    void onToolAI();
    void onToolEyedropper();
    void onToolFill();
    void onToolSelect();
    void onToolMove();

    // Help actions
    void onShowAbout();

private:
    void setupUi();
    void setupConnections();
    void setupDockWidgets();
    void setupToolActionGroup();
    void updateStatusBar();

    std::unique_ptr<Ui::MainWindow> ui;
    CanvasWidget* m_canvasWidget = nullptr;
    ToolsDockWidget* m_toolsDock = nullptr;
    ColorDockWidget* m_colorDock = nullptr;
    BrushSettingsDockWidget* m_brushSettingsDock = nullptr;
    LayersDockWidget* m_layersDock = nullptr;
    HistoryDockWidget* m_historyDock = nullptr;
    QActionGroup* m_toolActionGroup = nullptr;
};

} // namespace spriteai::editor::app
