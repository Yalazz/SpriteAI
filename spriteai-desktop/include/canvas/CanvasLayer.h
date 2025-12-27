#pragma once

#include <QRectF>
#include <QColor>
#include <QString>
#include <vector>

#include "core/Stroke.h"

class CanvasLayer
{
public:
    CanvasLayer() = default;
    explicit CanvasLayer(const QRectF& rect) : bounds(rect) {}

    QRectF bounds;                     // world-space
    QColor background = Qt::transparent;
    std::vector<Stroke> strokes;

    bool visible = true;
    float opacity = 1.0f;

    QString name = "Layer 1";
    QString ownerId = "local";
};
