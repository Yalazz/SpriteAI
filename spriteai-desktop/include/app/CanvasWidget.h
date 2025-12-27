#pragma once

#include <QWidget>
#include <memory>

class CpuCanvasRenderer;
class InfiniteGridRenderer;
class CanvasSession;

class CanvasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasWidget(QWidget* parent = nullptr);
    ~CanvasWidget();

protected:
    void paintEvent(QPaintEvent*) override;

    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;

    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;

private:
    void toggleGrid();
    void setInfiniteMode(bool enabled);
    void setBoundedMode();

private:
    std::unique_ptr<CanvasSession> session;
    std::unique_ptr<CpuCanvasRenderer> renderer;
    std::unique_ptr<InfiniteGridRenderer> gridRenderer;

    bool panMode = false;
    bool isPanning = false;
    bool isDrawing = false;

    QPointF lastPanPos;
};
