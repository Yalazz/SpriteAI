#include "spriteai/editor/app/ToolsDockWidget.h"
#include "ui_ToolsDock.h"

namespace spriteai::editor::app {

ToolsDockWidget::ToolsDockWidget(QWidget* parent)
    : QDockWidget(parent)
    , ui(std::make_unique<Ui::ToolsDock>())
{
    auto* widget = new QWidget(this);
    ui->setupUi(widget);
    setWidget(widget);

    setupConnections();
}

ToolsDockWidget::~ToolsDockWidget() = default;

void ToolsDockWidget::setupConnections()
{
    connect(ui->btnBrush, &QToolButton::clicked, this, &ToolsDockWidget::onBrushClicked);
    connect(ui->btnPencil, &QToolButton::clicked, this, &ToolsDockWidget::onPencilClicked);
    connect(ui->btnEraser, &QToolButton::clicked, this, &ToolsDockWidget::onEraserClicked);
    connect(ui->btnFill, &QToolButton::clicked, this, &ToolsDockWidget::onFillClicked);
    connect(ui->btnEyedropper, &QToolButton::clicked, this, &ToolsDockWidget::onEyedropperClicked);
    connect(ui->btnSelect, &QToolButton::clicked, this, &ToolsDockWidget::onSelectClicked);
    connect(ui->btnMove, &QToolButton::clicked, this, &ToolsDockWidget::onMoveClicked);
    connect(ui->btnAI, &QToolButton::clicked, this, &ToolsDockWidget::onAIClicked);
}

void ToolsDockWidget::setCurrentTool(const QString& toolId)
{
    ui->btnBrush->setChecked(toolId == "brush");
    ui->btnPencil->setChecked(toolId == "pencil");
    ui->btnEraser->setChecked(toolId == "eraser");
    ui->btnFill->setChecked(toolId == "fill");
    ui->btnEyedropper->setChecked(toolId == "eyedropper");
    ui->btnSelect->setChecked(toolId == "select");
    ui->btnMove->setChecked(toolId == "move");
    ui->btnAI->setChecked(toolId == "ai");
}

void ToolsDockWidget::onBrushClicked()
{
    emit toolSelected("brush");
}

void ToolsDockWidget::onPencilClicked()
{
    emit toolSelected("pencil");
}

void ToolsDockWidget::onEraserClicked()
{
    emit toolSelected("eraser");
}

void ToolsDockWidget::onFillClicked()
{
    emit toolSelected("fill");
}

void ToolsDockWidget::onEyedropperClicked()
{
    emit toolSelected("eyedropper");
}

void ToolsDockWidget::onSelectClicked()
{
    emit toolSelected("select");
}

void ToolsDockWidget::onMoveClicked()
{
    emit toolSelected("move");
}

void ToolsDockWidget::onAIClicked()
{
    emit toolSelected("ai");
}

} // namespace spriteai::editor::app
