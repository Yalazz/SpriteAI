#pragma once

#include "canvas/mode/CanvasMode.h"
#include <QSizeF>

/*
    Sınırlı canvas (Photoshop / Aseprite)
*/
class BoundedCanvasMode final : public CanvasMode
{
public:
    explicit BoundedCanvasMode(const QSizeF& canvasSize);

    bool isBounded() const override { return true; }
    bool hasInfiniteGrid() const override { return false; }

    void clampView(CanvasView& view,
                   const QSize& viewport) override;

private:
    QSizeF m_canvasSize;
};
