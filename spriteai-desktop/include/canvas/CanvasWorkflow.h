#pragma once

class CanvasView;

class CanvasWorkflow
{
public:
    virtual ~CanvasWorkflow() = default;

    // input izinleri
    virtual bool allowDrawing() const { return true; }
    virtual bool allowPan() const { return true; }
    virtual bool allowZoom() const { return true; }

    // görsel / davranış capability
    virtual bool hasInfiniteGrid() const { return false; }
    virtual bool isBoundedCanvas() const { return true; }

    // view clamp (bounded modda kullanacağız)
    virtual void clampView(CanvasView&) {}
};
