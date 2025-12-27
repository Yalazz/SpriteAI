#pragma once
#include "canvas/CanvasWorkflow.h"

class InfiniteGridWorkflow final : public CanvasWorkflow
{
public:
    bool allowDrawing() const override { return true; }
    bool allowPan() const override { return true; }
    bool allowZoom() const override { return true; }

    bool hasInfiniteGrid() const override { return true; }
    bool isBoundedCanvas() const override { return false; }
};
