#include "spriteai/editor/app/LayersDockWidget.h"
#include "ui_LayersDock.h"

#include <QInputDialog>

namespace spriteai::editor::app {

LayersDockWidget::LayersDockWidget(QWidget* parent)
    : QDockWidget(parent)
    , ui(std::make_unique<Ui::LayersDock>())
{
    auto* widget = new QWidget(this);
    ui->setupUi(widget);
    setWidget(widget);

    setupConnections();
}

LayersDockWidget::~LayersDockWidget() = default;

void LayersDockWidget::setupConnections()
{
    connect(ui->btnAddLayer, &QToolButton::clicked, this, &LayersDockWidget::onAddLayer);
    connect(ui->btnDeleteLayer, &QToolButton::clicked, this, &LayersDockWidget::onDeleteLayer);
    connect(ui->btnDuplicateLayer, &QToolButton::clicked, this, &LayersDockWidget::onDuplicateLayer);
    connect(ui->btnMoveUp, &QToolButton::clicked, this, &LayersDockWidget::onMoveUp);
    connect(ui->btnMoveDown, &QToolButton::clicked, this, &LayersDockWidget::onMoveDown);

    connect(ui->listLayers, &QListWidget::currentRowChanged, this, &LayersDockWidget::onLayerSelectionChanged);
    connect(ui->sliderLayerOpacity, &QSlider::valueChanged, this, &LayersDockWidget::onOpacitySliderChanged);
    connect(ui->comboBlendMode, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LayersDockWidget::onBlendModeChanged);
}

int LayersDockWidget::currentLayerIndex() const
{
    return ui->listLayers->currentRow();
}

void LayersDockWidget::setLayerOpacity(int index, int opacity)
{
    Q_UNUSED(index);
    ui->sliderLayerOpacity->setValue(opacity);
    ui->labelOpacityValue->setText(QString("%1%").arg(opacity));
}

void LayersDockWidget::onAddLayer()
{
    m_layerCount++;
    auto* item = new QListWidgetItem(
        QIcon(":/icons/icons/layers.svg"),
        QString("Layer %1").arg(m_layerCount)
    );
    ui->listLayers->insertItem(0, item);
    ui->listLayers->setCurrentRow(0);
    emit layerAdded();
}

void LayersDockWidget::onDeleteLayer()
{
    int row = ui->listLayers->currentRow();
    if (row >= 0 && ui->listLayers->count() > 1) {
        delete ui->listLayers->takeItem(row);
        emit layerDeleted(row);
    }
}

void LayersDockWidget::onDuplicateLayer()
{
    int row = ui->listLayers->currentRow();
    if (row >= 0) {
        auto* currentItem = ui->listLayers->item(row);
        auto* newItem = new QListWidgetItem(
            currentItem->icon(),
            currentItem->text() + " copy"
        );
        ui->listLayers->insertItem(row, newItem);
        ui->listLayers->setCurrentRow(row);
        emit layerDuplicated(row);
    }
}

void LayersDockWidget::onMoveUp()
{
    int row = ui->listLayers->currentRow();
    if (row > 0) {
        auto* item = ui->listLayers->takeItem(row);
        ui->listLayers->insertItem(row - 1, item);
        ui->listLayers->setCurrentRow(row - 1);
        emit layerMoved(row, row - 1);
    }
}

void LayersDockWidget::onMoveDown()
{
    int row = ui->listLayers->currentRow();
    if (row >= 0 && row < ui->listLayers->count() - 1) {
        auto* item = ui->listLayers->takeItem(row);
        ui->listLayers->insertItem(row + 1, item);
        ui->listLayers->setCurrentRow(row + 1);
        emit layerMoved(row, row + 1);
    }
}

void LayersDockWidget::onLayerSelectionChanged()
{
    int row = ui->listLayers->currentRow();
    if (row >= 0) {
        emit layerSelected(row);
    }
}

void LayersDockWidget::onOpacitySliderChanged(int value)
{
    ui->labelOpacityValue->setText(QString("%1%").arg(value));
    int row = ui->listLayers->currentRow();
    if (row >= 0) {
        emit layerOpacityChanged(row, value);
    }
}

void LayersDockWidget::onBlendModeChanged(int index)
{
    int row = ui->listLayers->currentRow();
    if (row >= 0) {
        emit blendModeChanged(row, ui->comboBlendMode->itemText(index));
    }
}

void LayersDockWidget::updateLayerList()
{
    // This would be called when layers change in the engine
}

} // namespace spriteai::editor::app
