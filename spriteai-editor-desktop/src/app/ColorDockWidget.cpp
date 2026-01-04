#include "spriteai/editor/app/ColorDockWidget.h"
#include "ui_ColorDock.h"

#include <QColorDialog>
#include <QPushButton>

namespace spriteai::editor::app {

ColorDockWidget::ColorDockWidget(QWidget* parent)
    : QDockWidget(parent)
    , ui(std::make_unique<Ui::ColorDock>())
{
    auto* widget = new QWidget(this);
    ui->setupUi(widget);
    setWidget(widget);

    setupConnections();
    setupPalette();
    updateColorPreview();
}

ColorDockWidget::~ColorDockWidget() = default;

void ColorDockWidget::setupConnections()
{
    // RGB sliders
    connect(ui->sliderR, &QSlider::valueChanged, this, &ColorDockWidget::onColorSliderChanged);
    connect(ui->sliderG, &QSlider::valueChanged, this, &ColorDockWidget::onColorSliderChanged);
    connect(ui->sliderB, &QSlider::valueChanged, this, &ColorDockWidget::onColorSliderChanged);
    connect(ui->sliderA, &QSlider::valueChanged, this, &ColorDockWidget::onColorSliderChanged);

    // Hex input
    connect(ui->editHex, &QLineEdit::editingFinished, this, &ColorDockWidget::onHexChanged);

    // Buttons
    connect(ui->btnSwapColors, &QPushButton::clicked, this, &ColorDockWidget::onSwapColors);
    connect(ui->btnPickColor, &QPushButton::clicked, this, &ColorDockWidget::onPickColor);
}

void ColorDockWidget::setupPalette()
{
    // Create a default palette
    QVector<QColor> defaultColors = {
        QColor("#000000"), QColor("#FFFFFF"), QColor("#FF0000"), QColor("#00FF00"),
        QColor("#0000FF"), QColor("#FFFF00"), QColor("#FF00FF"), QColor("#00FFFF"),
        QColor("#FF8000"), QColor("#8000FF"), QColor("#0080FF"), QColor("#FF0080"),
        QColor("#808080"), QColor("#C0C0C0"), QColor("#800000"), QColor("#008000"),
        QColor("#000080"), QColor("#808000"), QColor("#800080"), QColor("#008080"),
        QColor("#E94560"), QColor("#16213E"), QColor("#0F3460"), QColor("#1A1A2E")
    };

    auto* layout = ui->groupPalette->layout();
    if (auto* gridLayout = qobject_cast<QGridLayout*>(layout)) {
        int row = 0, col = 0;
        for (const auto& color : defaultColors) {
            auto* btn = new QPushButton(this);
            btn->setFixedSize(24, 24);
            btn->setStyleSheet(QString("background-color: %1; border: 1px solid #0f3460; border-radius: 4px;").arg(color.name()));
            btn->setToolTip(color.name());

            connect(btn, &QPushButton::clicked, this, [this, color]() {
                setForegroundColor(color);
            });

            gridLayout->addWidget(btn, row, col);
            col++;
            if (col >= 6) {
                col = 0;
                row++;
            }
        }
    }
}

void ColorDockWidget::setForegroundColor(const QColor& color)
{
    m_foregroundColor = color;
    updateSlidersFromColor(color);
    updateColorPreview();
    emit foregroundColorChanged(color);
}

void ColorDockWidget::setBackgroundColor(const QColor& color)
{
    m_backgroundColor = color;
    updateColorPreview();
    emit backgroundColorChanged(color);
}

void ColorDockWidget::onColorSliderChanged()
{
    if (m_updatingSliders) return;

    m_foregroundColor = QColor(
        ui->sliderR->value(),
        ui->sliderG->value(),
        ui->sliderB->value(),
        ui->sliderA->value()
    );

    m_updatingSliders = true;
    ui->editHex->setText(m_foregroundColor.name().mid(1).toUpper());
    m_updatingSliders = false;

    updateColorPreview();
    emit foregroundColorChanged(m_foregroundColor);
}

void ColorDockWidget::onHexChanged()
{
    if (m_updatingSliders) return;

    QString hex = ui->editHex->text();
    if (!hex.startsWith('#')) hex.prepend('#');

    QColor color(hex);
    if (color.isValid()) {
        setForegroundColor(color);
    }
}

void ColorDockWidget::onSwapColors()
{
    std::swap(m_foregroundColor, m_backgroundColor);
    updateSlidersFromColor(m_foregroundColor);
    updateColorPreview();
    emit foregroundColorChanged(m_foregroundColor);
    emit backgroundColorChanged(m_backgroundColor);
}

void ColorDockWidget::onPickColor()
{
    QColor color = QColorDialog::getColor(m_foregroundColor, this, tr("Select Color"),
        QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
        setForegroundColor(color);
    }
}

void ColorDockWidget::updateColorPreview()
{
    ui->foregroundColor->setStyleSheet(
        QString("background-color: %1; border-radius: 4px;").arg(m_foregroundColor.name()));
    ui->backgroundColor->setStyleSheet(
        QString("background-color: %1; border-radius: 4px;").arg(m_backgroundColor.name()));
}

void ColorDockWidget::updateSlidersFromColor(const QColor& color)
{
    m_updatingSliders = true;

    ui->sliderR->setValue(color.red());
    ui->sliderG->setValue(color.green());
    ui->sliderB->setValue(color.blue());
    ui->sliderA->setValue(color.alpha());

    ui->spinR->setValue(color.red());
    ui->spinG->setValue(color.green());
    ui->spinB->setValue(color.blue());
    ui->spinA->setValue(color.alpha());

    ui->editHex->setText(color.name().mid(1).toUpper());

    m_updatingSliders = false;
}

} // namespace spriteai::editor::app
