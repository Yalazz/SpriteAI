#pragma once

#include <vector>

class QWidget;

class CanvasRenderer
{
public:
    CanvasRenderer();
    ~CanvasRenderer();

    void resize(int w, int h);
    void render(QWidget* widget);

    void startStroke(float x, float y);
    void addPoint(float x, float y);
    void endStroke();

private:
    int m_width = 0;
    int m_height = 0;

    struct Point { float x, y; };
    std::vector<Point> m_currentStroke;
    std::vector<std::vector<Point>> m_strokes;
};
