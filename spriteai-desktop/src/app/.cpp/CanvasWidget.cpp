#include "../.h/CanvasWidget.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
#include <QInputDialog>
#include <QLineEdit>

#include "../../canvas/.h/CanvasManager.h"
#include "../../canvas/.h/CpuCanvasRenderer.h"
#include "../../canvas/.h/InfiniteGridRenderer.h"
#include "../../canvas/.h/CanvasState.h"

CanvasWidget::CanvasWidget(QWidget* parent)
    : QWidget(parent)
    , manager(std::make_unique<CanvasManager>())
    , gridRenderer(std::make_unique<InfiniteGridRenderer>())
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    // Başlangıç için 3 canvas
    manager->addCanvas("Canvas 1", std::make_unique<CpuCanvasRenderer>());
    manager->addCanvas("Canvas 2", std::make_unique<CpuCanvasRenderer>());
    manager->addCanvas("Canvas 3", std::make_unique<CpuCanvasRenderer>());
    manager->setActive(0);

    updateCanvasSizes();
}

CanvasWidget::~CanvasWidget() = default;

QPointF CanvasWidget::screenToWorld(const QPointF& p) const
{
    const CanvasState& S = manager->activeCanvas();
    return (p - S.cameraOffset) / S.zoom;
}

QPointF CanvasWidget::worldToScreen(const QPointF& p) const
{
    const CanvasState& S = manager->activeCanvas();
    return S.cameraOffset + p * S.zoom;
}

void CanvasWidget::updateCanvasSizes()
{
    const QSize s = size();

    for (int i = 0; i < manager->canvasCount(); ++i)
    {
        if (auto* st = manager->canvasAt(i))
        {
            if (st->renderer)
                st->renderer->setCanvasSize(s);
        }
    }
}

void CanvasWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    CanvasState& S = manager->activeCanvas();

    // GRID (screen-space + camera/zoom bilgisi)
    gridRenderer->render(p, S.zoom, S.cameraOffset, size());

    // STROKE’LAR (world-space → transform ile)
    if (S.renderer)
    {
        p.save();
        p.translate(S.cameraOffset);
        p.scale(S.zoom, S.zoom);
        S.renderer->renderToPainter(p);
        p.restore();
    }
}

void CanvasWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updateCanvasSizes();
    update();
}

void CanvasWidget::mousePressEvent(QMouseEvent* e)
{
    CanvasState& S = manager->activeCanvas();

    if (e->button() == Qt::LeftButton && panMode)
    {
        isPanning = true;
        lastPanPos = e->position();
        setCursor(Qt::ClosedHandCursor);
    }
    else if (e->button() == Qt::LeftButton && !panMode)
    {
        isDrawing = true;
        lastMousePos = e->position();

        if (S.renderer)
        {
            QPointF worldPos = screenToWorld(e->position());
            S.renderer->beginStroke(worldPos, QColor(40, 40, 40), 1.5f);
        }
    }

    QWidget::mousePressEvent(e);
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* e)
{
    CanvasState& S = manager->activeCanvas();

    if (isDrawing && S.renderer)
    {
        QPointF worldPos = screenToWorld(e->position());
        S.renderer->continueStroke(worldPos, 1.0f);
        lastMousePos = e->position();
        update();
    }
    else if (isPanning)
    {
        QPointF delta = e->position() - lastPanPos;
        lastPanPos = e->position();

        S.cameraOffset += delta;
        update();
    }

    QWidget::mouseMoveEvent(e);
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* e)
{
    CanvasState& S = manager->activeCanvas();

    if (e->button() == Qt::LeftButton && isDrawing && S.renderer)
    {
        S.renderer->endStroke();
        isDrawing = false;
        update();
    }

    if (e->button() == Qt::LeftButton && isPanning)
    {
        isPanning = false;
        if (panMode)
            setCursor(Qt::OpenHandCursor);
        else
            unsetCursor();
    }

    QWidget::mouseReleaseEvent(e);
}

void CanvasWidget::wheelEvent(QWheelEvent* e)
{
    CanvasState& S = manager->activeCanvas();

    const QPointF mouseScreen = e->position();
    const QPointF worldBefore = screenToWorld(mouseScreen);

    const int delta = e->angleDelta().y();
    const qreal factor = (delta > 0) ? 1.1 : 1.0 / 1.1;

    S.zoom *= factor;
    if (S.zoom < 0.1) S.zoom = 0.1;
    if (S.zoom > 20.0) S.zoom = 20.0;

    // Aynı world noktası mouse altında kalsın
    S.cameraOffset = mouseScreen - worldBefore * S.zoom;

    update();
    QWidget::wheelEvent(e);
}

void CanvasWidget::keyPressEvent(QKeyEvent* e)
{
    // PAN MODE
    if (e->key() == Qt::Key_Space)
    {
        panMode = true;
        if (!isPanning)
            setCursor(Qt::OpenHandCursor);
    }

    // === DİNAMİK HOTKEY CANVAS SWITCH (1–9) ===
    if (e->key() >= Qt::Key_1 && e->key() <= Qt::Key_9)
    {
        int index = e->key() - Qt::Key_1; // 0–8 arası

        if (index < manager->canvasCount())
        {
            manager->setActive(index);
            update();
            return;
        }
    }

    // === B → Yeni canvas oluştur ===
    if (e->key() == Qt::Key_B)
    {
        bool ok = false;
        QString name = QInputDialog::getText(
            this,
            "Yeni Canvas",
            "Canvas adı:",
            QLineEdit::Normal,
            "Untitled Canvas",
            &ok
        );

        if (ok && !name.isEmpty())
        {
            int newIndex = manager->addCanvas(
                name,
                std::make_unique<CpuCanvasRenderer>()
            );

            manager->setActive(newIndex);
            updateCanvasSizes();
            update();

            qDebug() << "Created new canvas:" << name << "index:" << newIndex;
        }
    }

    QWidget::keyPressEvent(e);
}


void CanvasWidget::keyReleaseEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Space)
    {
        panMode = false;
        if (!isPanning)
            unsetCursor();
    }

    QWidget::keyReleaseEvent(e);
}
