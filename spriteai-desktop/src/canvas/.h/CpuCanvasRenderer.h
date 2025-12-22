#pragma once
#include "../.h/ICanvasRenderer.h"
#include <vector>

struct StrokePoint {
    QPointF pos;
    float pressure;
};

struct Stroke
{
    QColor color;
    float width;
    std::vector<StrokePoint> points;
};

class CpuCanvasRenderer : public ICanvasRenderer
{
public:
    CpuCanvasRenderer();
    ~CpuCanvasRenderer() override;

    void setCanvasSize(const QSize& size) override;

    void beginStroke(const QPointF& worldPos,
                     const QColor& color,
                     float width) override;

    void continueStroke(const QPointF& worldPos,
                        float pressure = 1.0f) override;

    void endStroke() override;

    void clear(const QColor& color) override;

    void renderToPainter(QPainter& painter) override;

private:
    QSize canvasSize;

    std::vector<Stroke> strokes;
    Stroke current;

    bool drawing = false;
};
