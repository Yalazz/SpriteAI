#include "CanvasRenderer.h"
#include "Document.h"

#include <QPainter>

CanvasRenderer::CanvasRenderer() = default;
CanvasRenderer::~CanvasRenderer() = default;

void CanvasRenderer::setCanvasSize(const QSize& size)
{
    m_size = size;
}

void CanvasRenderer::render(QPainter& painter, const Document& document)
{
    painter.fillRect(QRect(QPoint(0, 0), m_size), Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);

    // Document içindeki tüm stroke'ları çiz
    for (const auto& stroke : document.strokes())
    {
        QPen pen(stroke.color, stroke.width);
        painter.setPen(pen);

        const auto& pts = stroke.points;
        for (int i = 1; i < pts.size(); ++i)
        {
            painter.drawLine(pts[i - 1], pts[i]);
        }
    }
}
