#pragma once

#include "canvas/mode/CanvasMode.h"

class InfiniteCanvasMode final : public CanvasMode
{
public:
    bool isBounded() const override { return false; }
    bool hasInfiniteGrid() const override { return true; }

    void clampView(CanvasView&, const QSize&) override
    {
        // sınır yok
    }
};
