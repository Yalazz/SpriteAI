#include "core/DrawingModel.h"

DrawingModel::DrawingModel(int width, int height)
    : m_width(width)
    , m_height(height)
{
}

int DrawingModel::width() const
{
    return m_width;
}

int DrawingModel::height() const
{
    return m_height;
}

void DrawingModel::beginStroke(const QColor& color, float width)
{
    m_strokes.push_back({});
    Stroke& s = m_strokes.back();
    s.color = color;
    s.width = width;
    m_activeStroke = &s;
}

void DrawingModel::addPoint(const QPointF& pos, float pressure)
{
    if (!m_activeStroke)
        return;

    m_activeStroke->points.push_back({ pos, pressure });
}

void DrawingModel::endStroke()
{
    m_activeStroke = nullptr;
}

const std::vector<Stroke>& DrawingModel::strokes() const
{
    return m_strokes;
}
