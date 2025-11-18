#pragma once

#include <vector>
#include <memory>

#include <QString>
#include "CanvasState.h"

class CanvasManager
{
public:
    CanvasManager();

    int addCanvas(const QString& name,
                  std::unique_ptr<ICanvasRenderer> renderer);

    void removeCanvas(int index);

    int  canvasCount() const { return static_cast<int>(canvases.size()); }

    CanvasState* canvasAt(int index)
    {
        if (index < 0 || index >= canvasCount()) return nullptr;
        return &canvases[index];
    }

    const CanvasState* canvasAt(int index) const
    {
        if (index < 0 || index >= canvasCount()) return nullptr;
        return &canvases[index];
    }

    int  activeIndex() const { return active; }
    void setActive(int index);

    CanvasState&       activeCanvas()       { return canvases[active]; }
    const CanvasState& activeCanvas() const { return canvases[active]; }

private:
    std::vector<CanvasState> canvases;
    int active = 0;
};
