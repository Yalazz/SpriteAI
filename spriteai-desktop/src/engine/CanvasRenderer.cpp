#include "CanvasRenderer.h"
#include <QPainter>
#include <QWidget>

CanvasRenderer::CanvasRenderer()
{
}

CanvasRenderer::~CanvasRenderer()
{
}

void CanvasRenderer::resize(int w, int h)
{
    m_width = w;
    m_height = h;
}

void CanvasRenderer::render(QWidget* widget)
{
    QPainter painter(widget);
    painter.fillRect(0, 0, m_width, m_height, Qt::white);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 2));

    // Completed strokes
    for (const auto& stroke : m_strokes)
        for (int i = 1; i < stroke.size(); ++i)
            painter.drawLine(stroke[i-1].x, stroke[i-1].y, stroke[i].x, stroke[i].y);

    // Current stroke
    for (int i = 1; i < m_currentStroke.size(); ++i)
        painter.drawLine(m_currentStroke[i-1].x, m_currentStroke[i-1].y,
                         m_currentStroke[i].x, m_currentStroke[i].y);
}

void CanvasRenderer::startStroke(float x, float y)
{
    m_currentStroke.clear();
    m_currentStroke.push_back({x, y});
}

void CanvasRenderer::addPoint(float x, float y)
{
    m_currentStroke.push_back({x, y});
}

void CanvasRenderer::endStroke()
{
    if (!m_currentStroke.empty())
        m_strokes.push_back(m_currentStroke);

    m_currentStroke.clear();
}
