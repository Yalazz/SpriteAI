#include "../.h/CpuCanvasRenderer.h"
#include <QPen>

CpuCanvasRenderer::CpuCanvasRenderer() {}

CpuCanvasRenderer::~CpuCanvasRenderer() {}

void CpuCanvasRenderer::setCanvasSize(const QSize& size)
{
    canvasSize = size;
}

void CpuCanvasRenderer::beginStroke(const QPointF& worldPos,
                                    const QColor& color,
                                    float width)
{
    drawing = true;

    current.color = color;
    current.width = width;
    current.points.clear();
    current.points.push_back({ worldPos, 1.0f });
}

void CpuCanvasRenderer::continueStroke(const QPointF& worldPos,
                                       float pressure)
{
    if (!drawing)
        return;

    current.points.push_back({ worldPos, pressure });
}

void CpuCanvasRenderer::endStroke()
{
    if (!drawing)
        return;

    drawing = false;

    if (!current.points.empty())
        strokes.push_back(current);

    current.points.clear();
}

void CpuCanvasRenderer::clear(const QColor&)
{
    strokes.clear();
    current.points.clear();
}

void CpuCanvasRenderer::renderToPainter(QPainter& p)
{
    // Past strokes
    for (const auto& s : strokes)
    {
        p.setPen(QPen(s.color, s.width,
                      Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        for (int i = 1; i < s.points.size(); ++i)
            p.drawLine(s.points[i - 1].pos,
                       s.points[i].pos);
    }

    // Current active stroke
    if (!current.points.empty())
    {
        p.setPen(QPen(current.color, current.width,
                      Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        for (int i = 1; i < current.points.size(); ++i)
            p.drawLine(current.points[i - 1].pos,
                       current.points[i].pos);
    }
}
