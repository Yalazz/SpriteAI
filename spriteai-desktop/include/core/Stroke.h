#pragma once

#include <QColor>
#include <QPointF>
#include <vector>

struct StrokePoint
{
    QPointF pos;
    float pressure = 1.0f;
};

struct Stroke
{
    QColor color = Qt::black;
    float width = 2.0f;
    std::vector<StrokePoint> points;
};
