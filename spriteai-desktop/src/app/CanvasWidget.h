#pragma once

#include <QWidget>
#include <QPointF>
#include <memory>

class CanvasManager;
class InfiniteGridRenderer;

class CanvasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasWidget(QWidget* parent = nullptr);
    ~CanvasWidget() override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    std::unique_ptr<CanvasManager>       manager;
    std::unique_ptr<InfiniteGridRenderer> gridRenderer;

    bool    isDrawing  = false;
    bool    isPanning  = false;
    bool    panMode    = false;   // Space basılı mı?

    QPointF lastPanPos;
    QPointF lastMousePos;

    QPointF screenToWorld(const QPointF& p) const;
    QPointF worldToScreen(const QPointF& p) const;

    void updateCanvasSizes();
};
