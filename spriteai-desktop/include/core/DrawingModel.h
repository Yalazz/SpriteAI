#pragma once

#include <vector>
#include <QColor>
#include <QPointF>

#include "core/Stroke.h"   // 🔥 SADECE BURADAN GELİR

class DrawingModel
{
public:
    DrawingModel(int width, int height);

    void beginStroke(const QColor& color, float width);
    void addPoint(const QPointF& pos, float pressure);
    void endStroke();

    const std::vector<Stroke>& strokes() const;

    int width() const;
    int height() const;

private:
    int m_width  = 0;
    int m_height = 0;

    std::vector<Stroke> m_strokes;
    Stroke* m_activeStroke = nullptr;
};
