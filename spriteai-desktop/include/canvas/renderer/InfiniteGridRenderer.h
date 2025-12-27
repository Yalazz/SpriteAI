#pragma once

#include <QPointF>
#include <QSize>

class QPainter;

class InfiniteGridRenderer
{
public:
    InfiniteGridRenderer() = default;
    ~InfiniteGridRenderer() = default;

    void render(QPainter& painter,
                qreal zoom,
                const QPointF& cameraOffset,
                const QSize& viewportSize);
};
