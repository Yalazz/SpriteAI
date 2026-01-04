#pragma once

#include <QObject>
#include <QDockWidget>
#include <QColor>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class ColorDock; }
QT_END_NAMESPACE

namespace spriteai::editor::app {

class ColorDockWidget final : public QDockWidget {
    Q_OBJECT

public:
    explicit ColorDockWidget(QWidget* parent = nullptr);
    ~ColorDockWidget() override;

    QColor foregroundColor() const { return m_foregroundColor; }
    QColor backgroundColor() const { return m_backgroundColor; }

    void setForegroundColor(const QColor& color);
    void setBackgroundColor(const QColor& color);

signals:
    void foregroundColorChanged(const QColor& color);
    void backgroundColorChanged(const QColor& color);

private slots:
    void onColorSliderChanged();
    void onHexChanged();
    void onSwapColors();
    void onPickColor();

private:
    void setupConnections();
    void updateColorPreview();
    void updateSlidersFromColor(const QColor& color);
    void setupPalette();

    std::unique_ptr<Ui::ColorDock> ui;
    QColor m_foregroundColor{233, 69, 96};
    QColor m_backgroundColor{26, 26, 46};
    bool m_updatingSliders = false;
};

} // namespace spriteai::editor::app
