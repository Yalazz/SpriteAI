#pragma once

#include <QColor>
#include <QPointF>
#include <QPainter>
#include <QSize>

class ICanvasRenderer
{
public:
    virtual ~ICanvasRenderer() = default;

    virtual void setCanvasSize(const QSize& size) = 0;

    // World-space başlangıç noktası
    virtual void beginStroke(const QPointF& worldPos,
                             const QColor& color,
                             float width) = 0;

    virtual void continueStroke(const QPointF& worldPos,
                                float pressure = 1.0f) = 0;

    virtual void endStroke() = 0;

    virtual void clear(const QColor& color) = 0;

    // Painter’a world-space’ten çizim yap
    virtual void renderToPainter(QPainter& painter) = 0;
};
