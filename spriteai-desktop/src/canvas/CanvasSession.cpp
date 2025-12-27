#include "canvas/CanvasSession.h"
#include "canvas/workflows/BoundedWorkflow.h"

CanvasSession::CanvasSession(int width, int height)
    : document(width, height)
    , m_workflow(std::make_unique<BoundedWorkflow>())
{
    layers.emplace_back(QRectF(0, 0, width, height));
}

void CanvasSession::setWorkflow(std::unique_ptr<CanvasWorkflow> wf)
{
    if (!wf)
        wf = std::make_unique<BoundedWorkflow>();

    m_workflow = std::move(wf);
    m_workflow->clampView(view);
}

const CanvasWorkflow& CanvasSession::workflow() const
{
    return *m_workflow;
}

CanvasWorkflow& CanvasSession::workflow()
{
    return *m_workflow;
}

CanvasLayer& CanvasSession::activeLayer()
{
    return layers.front();
}

const CanvasLayer& CanvasSession::activeLayer() const
{
    return layers.front();
}
