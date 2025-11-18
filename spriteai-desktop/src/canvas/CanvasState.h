#pragma once

#include <memory>
#include <QString>
#include <QPointF>

#include "ICanvasRenderer.h"

struct CanvasState
{
    QString name;
    std::unique_ptr<ICanvasRenderer> renderer;

    qreal   zoom         = 1.0;
    QPointF cameraOffset = QPointF(0.0, 0.0);   // world (0,0)’ın ekran koordinatı
};
