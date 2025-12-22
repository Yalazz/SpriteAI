#pragma once

#include <QPointF>
#include <QString>
#include <memory>

#include "ICanvasRenderer.h"

struct CanvasState
{
    QString name;
    std::unique_ptr<ICanvasRenderer> renderer;

    QPointF cameraOffset { 0.0, 0.0 };
    qreal   zoom         { 1.0 };

    CanvasState() = default;

    CanvasState(const QString& n,
                std::unique_ptr<ICanvasRenderer> r)
        : name(n)
        , renderer(std::move(r))
        , cameraOffset(0.0, 0.0)
        , zoom(1.0)
    {}
};
