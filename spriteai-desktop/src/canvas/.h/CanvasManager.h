#pragma once

#include "CanvasState.h"
#include <vector>
#include <QString>

class CanvasManager
{
public:
    CanvasManager();

    int addCanvas(const QString& name,
                  std::unique_ptr<ICanvasRenderer> renderer);

    void removeCanvas(int index);

    int canvasCount() const { return static_cast<int>(canvases.size()); }
    CanvasState* canvasAt(int index);

    CanvasState& activeCanvas();
    void setActive(int index);
    int  activeIndex() const { return active; }

private:
    std::vector<CanvasState> canvases;
    int active = 0;
};
