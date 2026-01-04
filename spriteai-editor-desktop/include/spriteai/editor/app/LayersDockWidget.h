#pragma once

#include <QObject>
#include <QDockWidget>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class LayersDock; }
QT_END_NAMESPACE

namespace spriteai::editor::app {

class LayersDockWidget final : public QDockWidget {
    Q_OBJECT

public:
    explicit LayersDockWidget(QWidget* parent = nullptr);
    ~LayersDockWidget() override;

    int currentLayerIndex() const;
    void setLayerOpacity(int index, int opacity);

signals:
    void layerSelected(int index);
    void layerAdded();
    void layerDeleted(int index);
    void layerDuplicated(int index);
    void layerMoved(int from, int to);
    void layerOpacityChanged(int index, int opacity);
    void blendModeChanged(int index, const QString& mode);

private slots:
    void onAddLayer();
    void onDeleteLayer();
    void onDuplicateLayer();
    void onMoveUp();
    void onMoveDown();
    void onLayerSelectionChanged();
    void onOpacitySliderChanged(int value);
    void onBlendModeChanged(int index);

private:
    void setupConnections();
    void updateLayerList();

    std::unique_ptr<Ui::LayersDock> ui;
    int m_layerCount = 1;
};

} // namespace spriteai::editor::app
