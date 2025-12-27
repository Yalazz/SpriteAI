#pragma once

#include <QSize>

class CanvasView;

/*
    CanvasMode =
    Canvas'ın "hangi dünyada" çalıştığını belirler.
*/
class CanvasMode
{
public:
    virtual ~CanvasMode() = default;

    // Kamera izinleri
    virtual bool allowPan() const  { return true; }
    virtual bool allowZoom() const { return true; }

    // Dünya tipi
    virtual bool isBounded() const = 0;
    virtual bool hasInfiniteGrid() const { return false; }

    // Kamera sınırlandırma (opsiyonel)
    virtual void clampView(CanvasView& view,
                           const QSize& viewport) = 0;
};
