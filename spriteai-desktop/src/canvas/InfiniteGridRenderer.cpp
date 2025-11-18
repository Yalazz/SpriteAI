#include "InfiniteGridRenderer.h"

#include <QtMath>

InfiniteGridRenderer::InfiniteGridRenderer()
    : gridStepWorld(32.0)
    , minorColor(230, 230, 230)
    , majorColor(210, 210, 210)
{
}

void InfiniteGridRenderer::render(QPainter& p,
                                  qreal zoom,
                                  const QPointF& cameraOffset,
                                  const QSize& widgetSize)
{
    if (zoom <= 0.0)
        return;

    const int w = widgetSize.width();
    const int h = widgetSize.height();

    // Arka plan
    p.fillRect(QRect(0, 0, w, h), QColor(245, 245, 245));

    // Ekrandaki world aralığı
    const qreal minWorldX = (0.0       - cameraOffset.x()) / zoom;
    const qreal maxWorldX = (qreal(w)  - cameraOffset.x()) / zoom;
    const qreal minWorldY = (0.0       - cameraOffset.y()) / zoom;
    const qreal maxWorldY = (qreal(h)  - cameraOffset.y()) / zoom;

    const qreal step = gridStepWorld;

    const int kMinX = static_cast<int>(std::floor(minWorldX / step)) - 1;
    const int kMaxX = static_cast<int>(std::ceil (maxWorldX / step)) + 1;
    const int kMinY = static_cast<int>(std::floor(minWorldY / step)) - 1;
    const int kMaxY = static_cast<int>(std::ceil (maxWorldY / step)) + 1;

    QPen pen;
    pen.setCosmetic(true);

    // Dikey çizgiler
    for (int k = kMinX; k <= kMaxX; ++k)
    {
        const qreal worldX  = k * step;
        const qreal screenX = cameraOffset.x() + worldX * zoom;

        if (k % 10 == 0)
        {
            pen.setColor(majorColor);
            pen.setWidthF(1.2);
        }
        else
        {
            pen.setColor(minorColor);
            pen.setWidthF(1.0);
        }

        p.setPen(pen);
        p.drawLine(QPointF(screenX, 0.0), QPointF(screenX, h));
    }

    // Yatay çizgiler
    for (int k = kMinY; k <= kMaxY; ++k)
    {
        const qreal worldY  = k * step;
        const qreal screenY = cameraOffset.y() + worldY * zoom;

        if (k % 10 == 0)
        {
            pen.setColor(majorColor);
            pen.setWidthF(1.2);
        }
        else
        {
            pen.setColor(minorColor);
            pen.setWidthF(1.0);
        }

        p.setPen(pen);
        p.drawLine(QPointF(0.0, screenY), QPointF(w, screenY));
    }
}
