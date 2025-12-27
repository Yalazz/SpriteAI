#pragma once
#include "theme/CanvasTheme.h"

class DarkTheme : public CanvasTheme
{
public:
    QColor canvasBackground() const override
    {
        return QColor(30, 30, 30);
    }

    QColor layerBackground() const override
    {
        return QColor(20, 20, 20);
    }

    QPen strokePen(const QColor& baseColor,
                   float width,
                   float pressure) const override
    {
        QPen pen(baseColor, width * pressure);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        return pen;
    }

    bool showGrid() const override { return true; }

    QColor gridColor() const override
    {
        return QColor(255, 255, 255, 25);
    }
};
