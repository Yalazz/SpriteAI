#include "../../../include/canvas/renderer/CpuCanvasRenderer.h"
#include "core/DrawingModel.h"

void CpuCanvasRenderer::setCanvasSize(const QSize& size)
{
    canvasSize = size;
}

void CpuCanvasRenderer::render(QPainter& p, const DrawingModel& doc)
{
    for (const auto& stroke : doc.strokes())
    {
        QPen pen(stroke.color, stroke.width);
        pen.setCapStyle(Qt::RoundCap);
        p.setPen(pen);

        for (size_t i = 1; i < stroke.points.size(); ++i)
        {
            p.drawLine(
                stroke.points[i - 1].pos,
                stroke.points[i].pos
            );
        }
    }
}
