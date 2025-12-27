#include "canvas/mode/BoundedCanvasMode.h"
#include "canvas/CanvasView.h"
#include <algorithm>

BoundedCanvasMode::BoundedCanvasMode(const QSizeF& canvasSize)
    : m_canvasSize(canvasSize)
{
}

void BoundedCanvasMode::clampView(CanvasView& view,
                                  const QSize& viewport)
{
    const qreal maxX =
        viewport.width()  - m_canvasSize.width()  * view.zoom;
    const qreal maxY =
        viewport.height() - m_canvasSize.height() * view.zoom;

    view.cameraOffset.setX(
        std::clamp(view.cameraOffset.x(), maxX, 0.0)
    );
    view.cameraOffset.setY(
        std::clamp(view.cameraOffset.y(), maxY, 0.0)
    );
}
