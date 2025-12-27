#include "app/CanvasWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <algorithm>

#include "canvas/CanvasSession.h"
#include "canvas/renderer/CpuCanvasRenderer.h"
#include "canvas/renderer/InfiniteGridRenderer.h"

#include "canvas/workflows/InfiniteGridWorkflow.h"
#include "canvas/workflows/BoundedWorkflow.h"

CanvasWidget::CanvasWidget(QWidget* parent)
    : QWidget(parent)
    , session(std::make_unique<CanvasSession>(1024, 1024))
    , renderer(std::make_unique<CpuCanvasRenderer>())
    , gridRenderer(std::make_unique<InfiniteGridRenderer>())
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    // default mod
    setBoundedMode();
}

CanvasWidget::~CanvasWidget() = default;

void CanvasWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.fillRect(rect(), Qt::white);

    // ---- GRID ----
    if (session->workflow().hasInfiniteGrid())
    {
        gridRenderer->render(
            p,
            session->view.zoom,
            session->view.cameraOffset,
            size()
        );
    }

    // ---- WORLD SPACE ----
    p.save();
    p.translate(session->view.cameraOffset);
    p.scale(session->view.zoom, session->view.zoom);

    renderer->render(p, session->document);

    p.restore();
}

void CanvasWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->button() != Qt::LeftButton)
        return;

    // Space + LMB => Pan
    if (panMode && session->workflow().allowPan())
    {
        isPanning = true;
        lastPanPos = e->position();
        setCursor(Qt::ClosedHandCursor);
        return;
    }

    // LMB => Draw
    if (session->workflow().allowDrawing())
    {
        isDrawing = true;
        session->document.beginStroke(QColor(40, 40, 40), 2.0f);
        session->document.addPoint(session->view.screenToWorld(e->position()), 1.0f);
        update();
    }
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* e)
{
    // Pan
    if (isPanning)
    {
        QPointF delta = e->position() - lastPanPos;
        lastPanPos = e->position();
        session->view.cameraOffset += delta;
        update();
        return;
    }

    // Draw (sol tuş basılı mı kontrol)
    if (isDrawing && (e->buttons() & Qt::LeftButton))
    {
        session->document.addPoint(session->view.screenToWorld(e->position()), 1.0f);
        update();
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() != Qt::LeftButton)
        return;

    if (isDrawing)
    {
        session->document.endStroke();
        isDrawing = false;
    }

    if (isPanning)
    {
        isPanning = false;
        panMode ? setCursor(Qt::OpenHandCursor) : unsetCursor();
    }

    update();
}

void CanvasWidget::wheelEvent(QWheelEvent* e)
{
    if (!session->workflow().allowZoom())
        return;

    const QPointF mouse = e->position();
    const QPointF worldBefore = session->view.screenToWorld(mouse);

    const qreal factor = (e->angleDelta().y() > 0) ? 1.1 : (1.0 / 1.1);
    session->view.zoom = std::clamp(session->view.zoom * factor, 0.1, 20.0);

    session->view.cameraOffset = mouse - worldBefore * session->view.zoom;

    session->workflow().clampView(session->view);
    update();
}

void CanvasWidget::keyPressEvent(QKeyEvent* e)
{
    if (e->isAutoRepeat())
        return;

    if (e->key() == Qt::Key_Space)
    {
        panMode = true;
        if (session->workflow().allowPan())
            setCursor(Qt::OpenHandCursor);
        return;
    }

    if (e->key() == Qt::Key_G) { toggleGrid(); return; }
    if (e->key() == Qt::Key_1) { setBoundedMode(); return; }
    if (e->key() == Qt::Key_2) { setInfiniteMode(true); return; }
    if (e->key() == Qt::Key_R) { session->view.reset(); update(); return; }
}

void CanvasWidget::keyReleaseEvent(QKeyEvent* e)
{
    if (e->isAutoRepeat())
        return;

    if (e->key() == Qt::Key_Space)
    {
        panMode = false;
        if (!isPanning)
            unsetCursor();
    }
}

void CanvasWidget::toggleGrid()
{
    if (!session->workflow().hasInfiniteGrid())
    {
        setInfiniteMode(true);
        return;
    }

    setBoundedMode();
}

void CanvasWidget::setInfiniteMode(bool enabled)
{
    (void)enabled;
    session->setWorkflow(std::make_unique<InfiniteGridWorkflow>());
    update();
}

void CanvasWidget::setBoundedMode()
{
    session->setWorkflow(std::make_unique<BoundedWorkflow>());
    update();
}
