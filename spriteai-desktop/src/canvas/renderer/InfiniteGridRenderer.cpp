#include "../../../include/canvas/renderer/InfiniteGridRenderer.h"

#include <QPainter>
#include <QPen>
#include <cmath>

void InfiniteGridRenderer::render(QPainter& painter,
                                  qreal zoom,
                                  const QPointF& cameraOffset,
                                  const QSize& viewportSize)
{
    painter.save();

    // Hafif grid
    QPen pen(QColor(220, 220, 220));
    pen.setWidthF(1.0);
    painter.setPen(pen);

    const qreal baseCell = 64.0;              // world-space hücre
    const qreal cellPx   = baseCell * zoom;   // screen-space

    if (cellPx < 8.0)
    {
        painter.restore();
        return;
    }

    const qreal w = viewportSize.width();
    const qreal h = viewportSize.height();

    const qreal x0 = std::fmod(cameraOffset.x(), cellPx);
    const qreal y0 = std::fmod(cameraOffset.y(), cellPx);

    for (qreal x = x0; x < w; x += cellPx)
        painter.drawLine(QPointF(x, 0), QPointF(x, h));

    for (qreal y = y0; y < h; y += cellPx)
        painter.drawLine(QPointF(0, y), QPointF(w, y));

    painter.restore();
}
