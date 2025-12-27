#pragma once
#include <QPointF>

class DrawingModel;

class Tool
{
public:
    virtual ~Tool() = default;

    virtual void onPress(DrawingModel& doc, const QPointF& worldPos) = 0;
    virtual void onMove (DrawingModel& doc, const QPointF& worldPos) = 0;
    virtual void onRelease(DrawingModel& doc, const QPointF& worldPos) = 0;
};
