#pragma once

#include <QSize>

class QPainter;
class Document;

class ICanvasRenderer
{
public:
    virtual ~ICanvasRenderer() = default;

    virtual void setCanvasSize(const QSize& size) = 0;
    virtual void render(QPainter& painter, const Document& document) = 0;
};
