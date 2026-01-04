#pragma once

#include <QObject>
#include <QDockWidget>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class BrushSettingsDock; }
QT_END_NAMESPACE

namespace spriteai::editor::app {

class BrushSettingsDockWidget final : public QDockWidget {
    Q_OBJECT

public:
    explicit BrushSettingsDockWidget(QWidget* parent = nullptr);
    ~BrushSettingsDockWidget() override;

    int brushSize() const;
    int brushOpacity() const;
    int brushSpacing() const;
    bool antialiasing() const;
    bool pressureAffectsSize() const;
    bool pressureAffectsOpacity() const;

    void setBrushSize(int size);
    void setBrushOpacity(int opacity);
    void setBrushSpacing(int spacing);

signals:
    void brushSizeChanged(int size);
    void brushOpacityChanged(int opacity);
    void brushSpacingChanged(int spacing);
    void antialiasingChanged(bool enabled);
    void pressureSizeChanged(bool enabled);
    void pressureOpacityChanged(bool enabled);

private slots:
    void onSizeChanged(int value);
    void onOpacityChanged(int value);
    void onSpacingChanged(int value);

private:
    void setupConnections();
    void updateBrushPreview();

    std::unique_ptr<Ui::BrushSettingsDock> ui;
};

} // namespace spriteai::editor::app
