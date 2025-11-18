#include "CpuCanvasRenderer.h"

#include <QDebug>

void CpuCanvasRenderer::beginStroke(const QPointF& worldPos,
                                    const QColor& color,
                                    float width)
{
    drawing = true;
    current = Stroke{};
    current.color = color;
    current.width = width;
    current.points.push_back({ worldPos, 1.0f });

    qDebug() << "[CPU] beginStroke at" << worldPos;
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

    qDebug() << "[CPU] endStroke, total strokes =" << strokes.size();
}

void CpuCanvasRenderer::clear(const QColor& /*color*/)
{
    strokes.clear();
    current.points.clear();
}

void CpuCanvasRenderer::renderToPainter(QPainter& p)
{
    p.setRenderHint(QPainter::Antialiasing, true);

    // Tamamlanmış stroke'lar
    for (const Stroke& s : strokes)
    {
        if (s.points.size() < 2)
            continue;

        QPen pen(s.color, s.width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        p.setPen(pen);

        for (int i = 1; i < s.points.size(); ++i)
        {
            const QPointF& p0 = s.points[i - 1].pos;
            const QPointF& p1 = s.points[i].pos;
            p.drawLine(p0, p1);
        }
    }

    // Aktif çizim
    if (current.points.size() >= 2)
    {
        QPen pen(current.color, current.width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        p.setPen(pen);

        for (int i = 1; i < current.points.size(); ++i)
        {
            const QPointF& p0 = current.points[i - 1].pos;
            const QPointF& p1 = current.points[i].pos;
            p.drawLine(p0, p1);
        }
    }
}
