#pragma once
#include "core/Tool.h"
#include <QColor>

class BrushTool : public Tool
{
public:
    BrushTool(QColor c = QColor(40,40,40), float s = 2.0f);

    void onPress  (DrawingModel&, const QPointF&) override;
    void onMove   (DrawingModel&, const QPointF&) override;
    void onRelease(DrawingModel&, const QPointF&) override;

private:
    QColor color;
    float  size;
};
