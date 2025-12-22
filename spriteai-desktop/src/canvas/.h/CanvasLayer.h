#pragma once
#include <QRectF>
#include <QColor>
#include <vector>
#include <QPointF>

struct Stroke {
    QColor color = Qt::black;
    float width = 2.0f;
    std::vector<QPointF> points;
};

class CanvasLayer {
public:
    QRectF bounds;                     // world-space canvas area
    QColor background = Qt::white;
    std::vector<Stroke> strokes;
    bool visible = true;
    QString ownerId = "local";

    CanvasLayer() = default;
    CanvasLayer(const QRectF& rect) : bounds(rect) {}
};
