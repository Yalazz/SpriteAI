#pragma once

#include <QPainter>
#include <QSize>

class DrawingModel;

class CpuCanvasRenderer
{
public:
    void setCanvasSize(const QSize& size);
    void render(QPainter& painter, const DrawingModel& document);

private:
    QSize canvasSize;
};
