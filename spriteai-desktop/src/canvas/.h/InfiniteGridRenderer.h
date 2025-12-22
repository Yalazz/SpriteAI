#pragma once

#include <QColor>
#include <QPointF>
#include <QPainter>
#include <QSize>

class InfiniteGridRenderer
{
public:
    InfiniteGridRenderer();

    void render(QPainter& p,
                qreal zoom,
                const QPointF& cameraOffset,
                const QSize& widgetSize);

    void setGridStepWorld(qreal step) { gridStepWorld = step; }

private:
    qreal  gridStepWorld;
    QColor minorColor;
    QColor majorColor;
};
