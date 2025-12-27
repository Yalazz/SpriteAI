#include "core/BrushTool.h"
#include "core/DrawingModel.h"

BrushTool::BrushTool(QColor c, float s)
    : color(c), size(s) {}

void BrushTool::onPress(DrawingModel& doc, const QPointF& p)
{
    doc.beginStroke(color, size);
    doc.addPoint(p, 1.0f);
}

void BrushTool::onMove(DrawingModel& doc, const QPointF& p)
{
    doc.addPoint(p, 1.0f);
}

void BrushTool::onRelease(DrawingModel& doc, const QPointF&)
{
    doc.endStroke();
}
