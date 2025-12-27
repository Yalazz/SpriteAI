#pragma once
#include <QColor>
#include <QPen>

class CanvasTheme
{
public:
    virtual ~CanvasTheme() = default;

    virtual QColor canvasBackground() const = 0;
    virtual QColor layerBackground() const = 0;

    virtual QPen strokePen(const QColor& baseColor,
                            float width,
                            float pressure) const = 0;

    virtual bool showGrid() const { return false; }
    virtual QColor gridColor() const { return Qt::lightGray; }
    virtual float gridSpacing() const { return 32.0f; }
};
