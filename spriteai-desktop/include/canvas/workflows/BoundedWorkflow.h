#pragma once
#include "canvas/CanvasWorkflow.h"
#include "canvas/CanvasView.h"
#include <algorithm>

class BoundedWorkflow final : public CanvasWorkflow
{
public:
    bool hasInfiniteGrid() const override { return false; }
    bool isBoundedCanvas() const override { return true; }

    void clampView(CanvasView& view) override
    {
        view.zoom = std::clamp(view.zoom, 0.1, 20.0);
    }
};
