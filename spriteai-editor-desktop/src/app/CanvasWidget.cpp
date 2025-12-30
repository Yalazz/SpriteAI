#include "spriteai/editor/app/CanvasWidget.h"
#include "spriteai/editor/app/ConfigHotReload.h"
#include "spriteai/editor/app/QtAIClient.h"

#include "spriteai/engine/EngineContext.h"
#include "spriteai/canvas/CanvasView.h"
#include "spriteai/engine/renderer/cpu/CpuCanvasRenderer.h"
#include "spriteai/core/tools/Tool.h"
#include "spriteai/core/tools/builtin/AITool.h"

#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDir>
#include <algorithm>

namespace spriteai::editor::app {

CanvasWidget::CanvasWidget(QWidget* parent)
    : QWidget(parent)
    , m_engine(std::make_unique<spriteai::engine::EngineContext>())
    , m_view(std::make_unique<spriteai::canvas::CanvasView>(
          &m_engine->document(), m_engine.get()))
    , m_renderer(std::make_unique<spriteai::engine::renderer::cpu::CpuCanvasRenderer>())
    , m_hotReload(std::make_unique<ConfigHotReload>(*m_engine, this))
{
    setFocusPolicy(Qt::StrongFocus);

    // ---- AI client injection (editor-side implementation)
    auto aiClient = std::make_shared<QtAIClient>();
    m_engine->setAIClient(aiClient);

    // ---- Hot reload
    m_hotReload->setProjectRoot(QDir::currentPath());
    m_hotReload->start();

    connect(m_hotReload.get(), &ConfigHotReload::configReloaded,
            this, [this] {
        if (auto* aiTool =
                dynamic_cast<spriteai::core::tools::builtin::AITool*>(
                    m_engine->activeTool())) {
            aiTool->setAIClient(m_engine->aiClient());
        }
        update();
    });
}

CanvasWidget::~CanvasWidget() = default;

// ------------------------------------------------------------
// Rendering
// ------------------------------------------------------------

void CanvasWidget::paintEvent(QPaintEvent*) {
    m_renderer->setCanvasSize(width(), height());
    m_renderer->render(*m_view);

    const auto* buf = m_renderer->rgba8Buffer();
    if (!buf) return;

    QImage img(reinterpret_cast<const uchar*>(buf),
               m_renderer->width(),
               m_renderer->height(),
               QImage::Format_RGBA8888);

    QPainter painter(this);
    painter.drawImage(0, 0, img);
}

// ------------------------------------------------------------
// Pointer handling
// ------------------------------------------------------------

    void CanvasWidget::sendPointer(bool down, QMouseEvent* e) {
    if (!m_engine) return;

    float wx, wy;
    m_view->screenToWorld(
        float(e->position().x()),
        float(e->position().y()),
        wx, wy
    );

    const bool alt   = e->modifiers() & Qt::AltModifier;
    const bool shift = e->modifiers() & Qt::ShiftModifier;

    if (down && e->type() == QEvent::MouseButtonPress) {
        m_engine->pointerDown(wx, wy, 1.0f, alt, shift);
    }
    else if (down && e->type() == QEvent::MouseMove) {
        m_engine->pointerMove(wx, wy, 1.0f, alt, shift);
    }
    else if (!down && e->type() == QEvent::MouseButtonRelease) {
        m_engine->pointerUp(wx, wy, 1.0f, alt, shift);
    }
}


void CanvasWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton)
        sendPointer(true, e);
    update();
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton)
        sendPointer(true, e);
    update();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton)
        sendPointer(false, e);
    update();
}

// ------------------------------------------------------------
// Camera
// ------------------------------------------------------------

void CanvasWidget::wheelEvent(QWheelEvent* e) {
    const float delta = float(e->angleDelta().y()) / 120.0f;
    auto& cam = m_view->camera();
    cam.zoom = std::clamp(
        cam.zoom * (1.0f + delta * 0.1f),
        0.1f,
        20.0f);
    update();
}

// ------------------------------------------------------------
// Keyboard
// ------------------------------------------------------------

void CanvasWidget::keyPressEvent(QKeyEvent* e) {
    // ---- Undo / Redo (UI-safe)
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->key() == Qt::Key_Z) {
            m_engine->undo();
            update();
            return;
        }
        if (e->key() == Qt::Key_Y) {
            m_engine->redo();
            update();
            return;
        }
    }

    // ---- Tool shortcuts
    const QString toolId = m_hotReload->toolIdForShortcut(e->key());
    if (!toolId.isEmpty()) {
        auto tool = m_hotReload->createToolById(toolId);
        if (tool) {
            if (auto* aiTool =
                    dynamic_cast<spriteai::core::tools::builtin::AITool*>(
                        tool.get())) {
                aiTool->setAIClient(m_engine->aiClient());
            }
            m_engine->setActiveTool(std::move(tool));
            update();
            return;
        }
    }

    QWidget::keyPressEvent(e);
}

} // namespace spriteai::editor::app
