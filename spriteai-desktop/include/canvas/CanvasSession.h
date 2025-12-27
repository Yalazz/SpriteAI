#pragma once

#include <memory>
#include <vector>
#include <QRectF>

#include "core/DrawingModel.h"
#include "canvas/CanvasView.h"
#include "canvas/CanvasWorkflow.h"
#include "canvas/CanvasLayer.h"

class CanvasSession
{
public:
    CanvasSession(int width, int height);

    void setWorkflow(std::unique_ptr<CanvasWorkflow> wf);
    const CanvasWorkflow& workflow() const;
    CanvasWorkflow& workflow();

    CanvasLayer& activeLayer();
    const CanvasLayer& activeLayer() const;

public:
    DrawingModel document;
    std::vector<CanvasLayer> layers;
    CanvasView view;

private:
    std::unique_ptr<CanvasWorkflow> m_workflow;
};
