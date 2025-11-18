#include "CanvasManager.h"

#include <QDebug>

CanvasManager::CanvasManager()
{
    qDebug() << "[CanvasManager] Initialized";
}

int CanvasManager::addCanvas(const QString& name,
                             std::unique_ptr<ICanvasRenderer> renderer)
{
    CanvasState st;
    st.name        = name;
    st.renderer    = std::move(renderer);
    st.zoom        = 1.0;
    st.cameraOffset = QPointF(0.0, 0.0);

    canvases.push_back(std::move(st));
    const int index = static_cast<int>(canvases.size()) - 1;

    qDebug() << "[CanvasManager] Canvas added:" << name << "index:" << index;
    return index;
}

void CanvasManager::removeCanvas(int index)
{
    if (index < 0 || index >= canvasCount())
        return;

    canvases.erase(canvases.begin() + index);

    if (canvases.empty())
    {
        active = 0;
    }
    else if (active >= canvasCount())
    {
        active = canvasCount() - 1;
    }
}

void CanvasManager::setActive(int index)
{
    if (index < 0 || index >= canvasCount())
        return;

    active = index;
    qDebug() << "[CanvasManager] Active canvas set to" << active;
}
