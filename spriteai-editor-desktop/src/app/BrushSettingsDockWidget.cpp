#include "spriteai/editor/app/BrushSettingsDockWidget.h"
#include "ui_BrushSettingsDock.h"

#include <QPainter>

namespace spriteai::editor::app {

BrushSettingsDockWidget::BrushSettingsDockWidget(QWidget* parent)
    : QDockWidget(parent)
    , ui(std::make_unique<Ui::BrushSettingsDock>())
{
    auto* widget = new QWidget(this);
    ui->setupUi(widget);
    setWidget(widget);

    setupConnections();
    updateBrushPreview();
}

BrushSettingsDockWidget::~BrushSettingsDockWidget() = default;

void BrushSettingsDockWidget::setupConnections()
{
    connect(ui->sliderSize, &QSlider::valueChanged, this, &BrushSettingsDockWidget::onSizeChanged);
    connect(ui->sliderOpacity, &QSlider::valueChanged, this, &BrushSettingsDockWidget::onOpacityChanged);
    connect(ui->sliderSpacing, &QSlider::valueChanged, this, &BrushSettingsDockWidget::onSpacingChanged);

    connect(ui->checkAntialiasing, &QCheckBox::toggled, this, &BrushSettingsDockWidget::antialiasingChanged);
    connect(ui->checkPressureSize, &QCheckBox::toggled, this, &BrushSettingsDockWidget::pressureSizeChanged);
    connect(ui->checkPressureOpacity, &QCheckBox::toggled, this, &BrushSettingsDockWidget::pressureOpacityChanged);
}

int BrushSettingsDockWidget::brushSize() const
{
    return ui->sliderSize->value();
}

int BrushSettingsDockWidget::brushOpacity() const
{
    return ui->sliderOpacity->value();
}

int BrushSettingsDockWidget::brushSpacing() const
{
    return ui->sliderSpacing->value();
}

bool BrushSettingsDockWidget::antialiasing() const
{
    return ui->checkAntialiasing->isChecked();
}

bool BrushSettingsDockWidget::pressureAffectsSize() const
{
    return ui->checkPressureSize->isChecked();
}

bool BrushSettingsDockWidget::pressureAffectsOpacity() const
{
    return ui->checkPressureOpacity->isChecked();
}

void BrushSettingsDockWidget::setBrushSize(int size)
{
    ui->sliderSize->setValue(size);
}

void BrushSettingsDockWidget::setBrushOpacity(int opacity)
{
    ui->sliderOpacity->setValue(opacity);
}

void BrushSettingsDockWidget::setBrushSpacing(int spacing)
{
    ui->sliderSpacing->setValue(spacing);
}

void BrushSettingsDockWidget::onSizeChanged(int value)
{
    updateBrushPreview();
    emit brushSizeChanged(value);
}

void BrushSettingsDockWidget::onOpacityChanged(int value)
{
    updateBrushPreview();
    emit brushOpacityChanged(value);
}

void BrushSettingsDockWidget::onSpacingChanged(int value)
{
    updateBrushPreview();
    emit brushSpacingChanged(value);
}

void BrushSettingsDockWidget::updateBrushPreview()
{
    // Create a preview pixmap
    QPixmap preview(ui->frameBrushPreview->width() - 4, ui->frameBrushPreview->height() - 4);
    preview.fill(QColor("#16213e"));

    QPainter painter(&preview);
    painter.setRenderHint(QPainter::Antialiasing, ui->checkAntialiasing->isChecked());

    // Draw brush preview
    int size = ui->sliderSize->value();
    int opacity = ui->sliderOpacity->value();

    QColor brushColor("#e94560");
    brushColor.setAlpha(opacity * 255 / 100);

    painter.setBrush(brushColor);
    painter.setPen(Qt::NoPen);

    int centerX = preview.width() / 2;
    int centerY = preview.height() / 2;

    painter.drawEllipse(QPoint(centerX, centerY), size / 2, size / 2);

    // Set the preview as background
    QPalette palette = ui->frameBrushPreview->palette();
    palette.setBrush(QPalette::Window, preview);
    ui->frameBrushPreview->setPalette(palette);
    ui->frameBrushPreview->setAutoFillBackground(true);
}

} // namespace spriteai::editor::app
