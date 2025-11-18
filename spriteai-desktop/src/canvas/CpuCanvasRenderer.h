#pragma once

#include "ICanvasRenderer.h"

#include <vector>

struct StrokePoint
{
    QPointF pos;
    float   pressure = 1.0f;
};

struct Stroke
{
    QColor color;
    float  width = 1.0f;
    std::vector<StrokePoint> points;
};

class CpuCanvasRenderer : public ICanvasRenderer
{
public:
    CpuCanvasRenderer() = default;

    void setCanvasSize(const QSize& size) override
    {
        canvasSize = size;
    }

    void beginStroke(const QPointF& worldPos,
                     const QColor& color,
                     float width) override;

    void continueStroke(const QPointF& worldPos,
                        float pressure = 1.0f) override;

    void endStroke() override;

    void clear(const QColor& /*color*/) override;

    void renderToPainter(QPainter& painter) override;

    const std::vector<Stroke>& getStrokes() const { return strokes; }
    const Stroke&              getCurrent() const { return current; }

private:
    QSize canvasSize;

    std::vector<Stroke> strokes;
    Stroke              current;
    bool                drawing = false;
};
