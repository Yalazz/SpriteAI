#pragma once

#include <vector>
#include <memory>

struct CanvasSession;

class CanvasManager
{
public:
    int addCanvas(std::unique_ptr<CanvasSession> canvas);
    void removeCanvas(int index);

    int canvasCount() const;

    CanvasSession& canvasAt(int index);
    CanvasSession& activeCanvas();

    void setActive(int index);
    int activeIndex() const;

private:
    std::vector<std::unique_ptr<CanvasSession>> canvases;
    int active = 0;
};
