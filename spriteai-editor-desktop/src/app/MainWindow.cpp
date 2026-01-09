#include "spriteai/editor/app/MainWindow.h"
#include "spriteai/editor/app/CanvasWidget.h"
#include "spriteai/editor/app/ToolsDockWidget.h"
#include "spriteai/editor/app/ColorDockWidget.h"
#include "spriteai/editor/app/BrushSettingsDockWidget.h"
#include "spriteai/editor/app/LayersDockWidget.h"
#include "spriteai/editor/app/HistoryDockWidget.h"
#include "ui_MainWindow.h"

#include <QActionGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

namespace spriteai::editor::app {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    setupUi();
    setupDockWidgets();
    setupConnections();
    setupToolActionGroup();
    updateStatusBar();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi()
{
    ui->setupUi(this);

    // Create canvas widget and set as central widget
    m_canvasWidget = new CanvasWidget(this);
    ui->centralWidget->layout()->addWidget(m_canvasWidget);

    // Set window properties
    setWindowTitle("Sprite AI");
    resize(1400, 900);
}

void MainWindow::setupDockWidgets()
{
    // Tools dock (left side)
    m_toolsDock = new ToolsDockWidget(this);
    m_toolsDock->setWindowTitle(tr("Tools"));
    m_toolsDock->setObjectName("ToolsDock");
    addDockWidget(Qt::LeftDockWidgetArea, m_toolsDock);

    // Colors dock (right side)
    m_colorDock = new ColorDockWidget(this);
    m_colorDock->setWindowTitle(tr("Colors"));
    m_colorDock->setObjectName("ColorsDock");
    addDockWidget(Qt::RightDockWidgetArea, m_colorDock);

    // Brush settings dock (right side, below colors)
    m_brushSettingsDock = new BrushSettingsDockWidget(this);
    m_brushSettingsDock->setWindowTitle(tr("Brush Settings"));
    m_brushSettingsDock->setObjectName("BrushSettingsDock");
    addDockWidget(Qt::RightDockWidgetArea, m_brushSettingsDock);

    // Layers dock (right side, below brush settings)
    m_layersDock = new LayersDockWidget(this);
    m_layersDock->setWindowTitle(tr("Layers"));
    m_layersDock->setObjectName("LayersDock");
    addDockWidget(Qt::RightDockWidgetArea, m_layersDock);

    // History dock (right side, below layers)
    m_historyDock = new HistoryDockWidget(this);
    m_historyDock->setWindowTitle(tr("History"));
    m_historyDock->setObjectName("HistoryDock");
    addDockWidget(Qt::RightDockWidgetArea, m_historyDock);

    // Tab the right-side docks together
    tabifyDockWidget(m_layersDock, m_historyDock);
    m_layersDock->raise();

    // Connect dock visibility to menu actions
    ui->actionShowTools->setChecked(m_toolsDock->isVisible());
    ui->actionShowColors->setChecked(m_colorDock->isVisible());
    ui->actionShowBrushSettings->setChecked(m_brushSettingsDock->isVisible());
    ui->actionShowLayers->setChecked(m_layersDock->isVisible());
    ui->actionShowHistory->setChecked(m_historyDock->isVisible());

    connect(ui->actionShowTools, &QAction::toggled, m_toolsDock, &QDockWidget::setVisible);
    connect(ui->actionShowColors, &QAction::toggled, m_colorDock, &QDockWidget::setVisible);
    connect(ui->actionShowBrushSettings, &QAction::toggled, m_brushSettingsDock, &QDockWidget::setVisible);
    connect(ui->actionShowLayers, &QAction::toggled, m_layersDock, &QDockWidget::setVisible);
    connect(ui->actionShowHistory, &QAction::toggled, m_historyDock, &QDockWidget::setVisible);

    connect(m_toolsDock, &QDockWidget::visibilityChanged, ui->actionShowTools, &QAction::setChecked);
    connect(m_colorDock, &QDockWidget::visibilityChanged, ui->actionShowColors, &QAction::setChecked);
    connect(m_brushSettingsDock, &QDockWidget::visibilityChanged, ui->actionShowBrushSettings, &QAction::setChecked);
    connect(m_layersDock, &QDockWidget::visibilityChanged, ui->actionShowLayers, &QAction::setChecked);
    connect(m_historyDock, &QDockWidget::visibilityChanged, ui->actionShowHistory, &QAction::setChecked);
}

void MainWindow::setupConnections()
{
    // File menu
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewFile);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveFileAs);
    connect(ui->actionExport, &QAction::triggered, this, &MainWindow::onExportFile);
    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);

    // Edit menu
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::onUndo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::onRedo);

    // View menu
    connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::onZoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::onZoomOut);
    connect(ui->actionZoomFit, &QAction::triggered, this, &MainWindow::onZoomFit);
    connect(ui->actionZoom100, &QAction::triggered, this, &MainWindow::onZoom100);
    connect(ui->actionShowGrid, &QAction::toggled, this, &MainWindow::onToggleGrid);
    connect(ui->actionShowSymmetry, &QAction::toggled, this, &MainWindow::onToggleSymmetry);

    // Tools menu
    connect(ui->actionBrush, &QAction::triggered, this, &MainWindow::onToolBrush);
    connect(ui->actionPencil, &QAction::triggered, this, &MainWindow::onToolPencil);
    connect(ui->actionEraser, &QAction::triggered, this, &MainWindow::onToolEraser);
    connect(ui->actionAITool, &QAction::triggered, this, &MainWindow::onToolAI);
    connect(ui->actionEyedropper, &QAction::triggered, this, &MainWindow::onToolEyedropper);
    connect(ui->actionFill, &QAction::triggered, this, &MainWindow::onToolFill);
    connect(ui->actionSelect, &QAction::triggered, this, &MainWindow::onToolSelect);
    connect(ui->actionMove, &QAction::triggered, this, &MainWindow::onToolMove);

    // Help menu
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onShowAbout);

    // Tools dock
    connect(m_toolsDock, &ToolsDockWidget::toolSelected, this, [this](const QString& toolId) {
        if (toolId == "brush") onToolBrush();
        else if (toolId == "pencil") onToolPencil();
        else if (toolId == "eraser") onToolEraser();
        else if (toolId == "ai") onToolAI();
        else if (toolId == "eyedropper") onToolEyedropper();
        else if (toolId == "fill") onToolFill();
        else if (toolId == "select") onToolSelect();
        else if (toolId == "move") onToolMove();
    });

    // History dock
    connect(m_historyDock, &HistoryDockWidget::undoRequested, this, &MainWindow::onUndo);
    connect(m_historyDock, &HistoryDockWidget::redoRequested, this, &MainWindow::onRedo);

    // Color dock → Canvas
    connect(m_colorDock, &ColorDockWidget::foregroundColorChanged, this, [this](const QColor& color) {
        if (m_canvasWidget) {
            m_canvasWidget->setForegroundColor(color);
        }
    });
    connect(m_colorDock, &ColorDockWidget::backgroundColorChanged, this, [this](const QColor& color) {
        if (m_canvasWidget) {
            m_canvasWidget->setBackgroundColor(color);
        }
    });

    // Brush settings dock → Canvas
    connect(m_brushSettingsDock, &BrushSettingsDockWidget::brushSizeChanged, this, [this](int size) {
        if (m_canvasWidget) {
            m_canvasWidget->setBrushSize(size);
        }
    });
    connect(m_brushSettingsDock, &BrushSettingsDockWidget::brushOpacityChanged, this, [this](int opacity) {
        if (m_canvasWidget) {
            m_canvasWidget->setBrushOpacity(opacity);
        }
    });
    connect(m_brushSettingsDock, &BrushSettingsDockWidget::brushSpacingChanged, this, [this](int spacing) {
        if (m_canvasWidget) {
            m_canvasWidget->setBrushSpacing(spacing);
        }
    });

    // Layers dock → Canvas
    connect(m_layersDock, &LayersDockWidget::layerAdded, this, [this]() {
        if (m_canvasWidget) {
            m_canvasWidget->addLayer();
        }
    });
    connect(m_layersDock, &LayersDockWidget::layerDeleted, this, [this](int index) {
        if (m_canvasWidget) {
            m_canvasWidget->deleteLayer(index);
        }
    });
    connect(m_layersDock, &LayersDockWidget::layerDuplicated, this, [this](int index) {
        if (m_canvasWidget) {
            m_canvasWidget->duplicateLayer(index);
        }
    });
    connect(m_layersDock, &LayersDockWidget::layerMoved, this, [this](int from, int to) {
        if (m_canvasWidget) {
            m_canvasWidget->moveLayer(from, to);
        }
    });
    connect(m_layersDock, &LayersDockWidget::layerSelected, this, [this](int index) {
        if (m_canvasWidget) {
            m_canvasWidget->setActiveLayer(index);
        }
    });
    connect(m_layersDock, &LayersDockWidget::layerOpacityChanged, this, [this](int index, int opacity) {
        if (m_canvasWidget) {
            m_canvasWidget->setLayerOpacity(index, opacity);
        }
    });
    connect(m_layersDock, &LayersDockWidget::blendModeChanged, this, [this](int index, const QString& mode) {
        if (m_canvasWidget) {
            m_canvasWidget->setLayerBlendMode(index, mode);
        }
    });

    // Edit menu - additional actions
    connect(ui->actionSelectAll, &QAction::triggered, this, &MainWindow::onSelectAll);
    connect(ui->actionDeselectAll, &QAction::triggered, this, &MainWindow::onDeselectAll);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDelete);

    // Image menu
    connect(ui->actionFlipHorizontal, &QAction::triggered, this, &MainWindow::onFlipHorizontal);
    connect(ui->actionFlipVertical, &QAction::triggered, this, &MainWindow::onFlipVertical);
    connect(ui->actionRotate90CW, &QAction::triggered, this, &MainWindow::onRotate90CW);
    connect(ui->actionRotate90CCW, &QAction::triggered, this, &MainWindow::onRotate90CCW);
}

void MainWindow::setupToolActionGroup()
{
    m_toolActionGroup = new QActionGroup(this);
    m_toolActionGroup->addAction(ui->actionBrush);
    m_toolActionGroup->addAction(ui->actionPencil);
    m_toolActionGroup->addAction(ui->actionEraser);
    m_toolActionGroup->addAction(ui->actionAITool);
    m_toolActionGroup->addAction(ui->actionEyedropper);
    m_toolActionGroup->addAction(ui->actionFill);
    m_toolActionGroup->addAction(ui->actionSelect);
    m_toolActionGroup->addAction(ui->actionMove);
    m_toolActionGroup->setExclusive(true);

    ui->actionBrush->setChecked(true);
}

void MainWindow::updateStatusBar()
{
    auto* zoomLabel = new QLabel("100%", this);
    zoomLabel->setMinimumWidth(60);
    zoomLabel->setAlignment(Qt::AlignCenter);
    ui->statusBar->addPermanentWidget(zoomLabel);

    auto* posLabel = new QLabel("X: 0  Y: 0", this);
    posLabel->setMinimumWidth(100);
    ui->statusBar->addPermanentWidget(posLabel);

    auto* sizeLabel = new QLabel("64 x 64", this);
    sizeLabel->setMinimumWidth(80);
    ui->statusBar->addPermanentWidget(sizeLabel);

    ui->statusBar->showMessage(tr("Ready"));
}

// File actions
void MainWindow::onNewFile()
{
    if (m_canvasWidget) {
        m_canvasWidget->clearCanvas();
    }
    ui->statusBar->showMessage(tr("New file created"), 2000);
}

void MainWindow::onOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Sprite"), QString(),
        tr("Sprite Files (*.sprite *.png *.gif);;All Files (*)"));
    if (!fileName.isEmpty()) {
        ui->statusBar->showMessage(tr("Opened: %1").arg(fileName), 2000);
    }
}

void MainWindow::onSaveFile()
{
    ui->statusBar->showMessage(tr("File saved"), 2000);
}

void MainWindow::onSaveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Sprite"), QString(),
        tr("Sprite Files (*.sprite);;PNG Files (*.png);;All Files (*)"));
    if (!fileName.isEmpty()) {
        ui->statusBar->showMessage(tr("Saved as: %1").arg(fileName), 2000);
    }
}

void MainWindow::onExportFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Export Sprite"), QString(),
        tr("PNG Files (*.png);;GIF Files (*.gif);;All Files (*)"));
    if (!fileName.isEmpty()) {
        ui->statusBar->showMessage(tr("Exported: %1").arg(fileName), 2000);
    }
}

// Edit actions
void MainWindow::onUndo()
{
    if (m_canvasWidget) {
        m_canvasWidget->undo();
    }
    ui->statusBar->showMessage(tr("Undo"), 1000);
}

void MainWindow::onRedo()
{
    if (m_canvasWidget) {
        m_canvasWidget->redo();
    }
    ui->statusBar->showMessage(tr("Redo"), 1000);
}

// View actions
void MainWindow::onZoomIn()
{
    if (m_canvasWidget) {
        m_canvasWidget->zoomIn();
    }
    ui->statusBar->showMessage(tr("Zoom In"), 1000);
}

void MainWindow::onZoomOut()
{
    if (m_canvasWidget) {
        m_canvasWidget->zoomOut();
    }
    ui->statusBar->showMessage(tr("Zoom Out"), 1000);
}

void MainWindow::onZoomFit()
{
    if (m_canvasWidget) {
        m_canvasWidget->zoomFit();
    }
    ui->statusBar->showMessage(tr("Zoom to Fit"), 1000);
}

void MainWindow::onZoom100()
{
    if (m_canvasWidget) {
        m_canvasWidget->setZoom(1.0f);
    }
    ui->statusBar->showMessage(tr("Zoom 100%"), 1000);
}

void MainWindow::onToggleGrid(bool checked)
{
    if (m_canvasWidget) {
        m_canvasWidget->setGridEnabled(checked);
    }
    ui->statusBar->showMessage(checked ? tr("Grid enabled") : tr("Grid disabled"), 1000);
}

void MainWindow::onToggleSymmetry(bool checked)
{
    if (m_canvasWidget) {
        m_canvasWidget->setSymmetryEnabled(checked);
    }
    ui->statusBar->showMessage(checked ? tr("Symmetry enabled") : tr("Symmetry disabled"), 1000);
}

// Edit actions (additional)
void MainWindow::onSelectAll()
{
    if (m_canvasWidget) {
        m_canvasWidget->selectAll();
    }
    ui->statusBar->showMessage(tr("Selected all"), 1000);
}

void MainWindow::onDeselectAll()
{
    if (m_canvasWidget) {
        m_canvasWidget->deselectAll();
    }
    ui->statusBar->showMessage(tr("Deselected all"), 1000);
}

void MainWindow::onDelete()
{
    if (m_canvasWidget) {
        m_canvasWidget->deleteSelection();
    }
    ui->statusBar->showMessage(tr("Deleted selection"), 1000);
}

// Image actions
void MainWindow::onFlipHorizontal()
{
    if (m_canvasWidget) {
        m_canvasWidget->flipHorizontal();
    }
    ui->statusBar->showMessage(tr("Flipped horizontally"), 1000);
}

void MainWindow::onFlipVertical()
{
    if (m_canvasWidget) {
        m_canvasWidget->flipVertical();
    }
    ui->statusBar->showMessage(tr("Flipped vertically"), 1000);
}

void MainWindow::onRotate90CW()
{
    if (m_canvasWidget) {
        m_canvasWidget->rotate90CW();
    }
    ui->statusBar->showMessage(tr("Rotated 90° clockwise"), 1000);
}

void MainWindow::onRotate90CCW()
{
    if (m_canvasWidget) {
        m_canvasWidget->rotate90CCW();
    }
    ui->statusBar->showMessage(tr("Rotated 90° counter-clockwise"), 1000);
}

// Tool actions
void MainWindow::onToolBrush()
{
    if (m_canvasWidget) {
        m_canvasWidget->setTool("brush");
    }
    ui->actionBrush->setChecked(true);
    m_toolsDock->setCurrentTool("brush");
    ui->statusBar->showMessage(tr("Brush Tool"), 1000);
}

void MainWindow::onToolPencil()
{
    if (m_canvasWidget) {
        m_canvasWidget->setTool("pencil");
    }
    ui->actionPencil->setChecked(true);
    m_toolsDock->setCurrentTool("pencil");
    ui->statusBar->showMessage(tr("Pencil Tool"), 1000);
}

void MainWindow::onToolEraser()
{
    if (m_canvasWidget) {
        m_canvasWidget->setTool("eraser");
    }
    ui->actionEraser->setChecked(true);
    m_toolsDock->setCurrentTool("eraser");
    ui->statusBar->showMessage(tr("Eraser Tool"), 1000);
}

void MainWindow::onToolAI()
{
    if (m_canvasWidget) {
        m_canvasWidget->setTool("ai_clean");
    }
    ui->actionAITool->setChecked(true);
    m_toolsDock->setCurrentTool("ai");
    ui->statusBar->showMessage(tr("AI Tool"), 1000);
}

void MainWindow::onToolEyedropper()
{
    if (m_canvasWidget) {
        m_canvasWidget->setTool("eyedropper");
    }
    ui->actionEyedropper->setChecked(true);
    m_toolsDock->setCurrentTool("eyedropper");
    ui->statusBar->showMessage(tr("Eyedropper Tool"), 1000);
}

void MainWindow::onToolFill()
{
    if (m_canvasWidget) {
        m_canvasWidget->setTool("fill");
    }
    ui->actionFill->setChecked(true);
    m_toolsDock->setCurrentTool("fill");
    ui->statusBar->showMessage(tr("Fill Tool"), 1000);
}

void MainWindow::onToolSelect()
{
    if (m_canvasWidget) {
        m_canvasWidget->setTool("select");
    }
    ui->actionSelect->setChecked(true);
    m_toolsDock->setCurrentTool("select");
    ui->statusBar->showMessage(tr("Selection Tool"), 1000);
}

void MainWindow::onToolMove()
{
    if (m_canvasWidget) {
        m_canvasWidget->setTool("move");
    }
    ui->actionMove->setChecked(true);
    m_toolsDock->setCurrentTool("move");
    ui->statusBar->showMessage(tr("Move Tool"), 1000);
}

// Help actions
void MainWindow::onShowAbout()
{
    QMessageBox::about(this, tr("About Sprite AI"),
        tr("<h2>Sprite AI</h2>"
           "<p>Version 1.0</p>"
           "<p>An AI-powered sprite art editor with modular architecture.</p>"
           "<p>Built with Qt and C++20.</p>"));
}

} // namespace spriteai::editor::app
