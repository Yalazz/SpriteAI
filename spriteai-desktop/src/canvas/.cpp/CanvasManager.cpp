#include "../.h/CanvasManager.h"
#include <QDebug>

CanvasManager::CanvasManager() = default;

int CanvasManager::addCanvas(const QString& name,
                             std::unique_ptr<ICanvasRenderer> renderer)
{
    CanvasState st(name, std::move(renderer));
    canvases.push_back(std::move(st));

    int idx = static_cast<int>(canvases.size()) - 1;

    if (idx == 0)
        active = 0;

    qDebug() << "[CanvasManager] addCanvas:" << name << "index:" << idx;
    return idx;
}

void CanvasManager::removeCanvas(int index)
{
    if (index < 0 || index >= canvasCount())
        return;

    canvases.erase(canvases.begin() + index);

    if (canvases.empty())
    {
        active = 0;
        return;
    }

    if (active >= canvasCount())
        active = canvasCount() - 1;
}

CanvasState* CanvasManager::canvasAt(int index)
{
    if (index < 0 || index >= canvasCount())
        return nullptr;

    return &canvases[index];
}

CanvasState& CanvasManager::activeCanvas()
{
    return canvases[active];
}

void CanvasManager::setActive(int index)
{
    if (index < 0 || index >= canvasCount())
        return;

    active = index;
    qDebug() << "[CanvasManager] Active canvas set to" << active;
}
