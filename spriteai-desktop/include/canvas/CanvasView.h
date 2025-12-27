#pragma once
#include <QPointF>

class CanvasView
{
public:
    QPointF cameraOffset {0.0, 0.0};
    qreal   zoom {1.0};

    QPointF screenToWorld(const QPointF& p) const
    {
        return (p - cameraOffset) / zoom;
    }

    QPointF worldToScreen(const QPointF& p) const
    {
        return p * zoom + cameraOffset;
    }

    void reset()
    {
        cameraOffset = {0.0, 0.0};
        zoom = 1.0;
    }
};
