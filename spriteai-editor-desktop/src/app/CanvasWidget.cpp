#include "spriteai/editor/app/CanvasWidget.h"
#include "spriteai/editor/app/ConfigHotReload.h"
#include "spriteai/editor/app/QtAIClient.h"

#include "spriteai/engine/EngineContext.h"
#include "spriteai/canvas/CanvasView.h"
#include "spriteai/engine/renderer/cpu/CpuCanvasRenderer.h"
#include "spriteai/core/tools/Tool.h"
#include "spriteai/core/tools/ToolContext.h"
#include "spriteai/core/tools/builtin/AITool.h"
#include "spriteai/core/tools/builtin/BrushTool.h"
#include "spriteai/core/tools/builtin/PencilTool.h"
#include "spriteai/core/tools/builtin/FillTool.h"
#include "spriteai/core/selection/Selection.h"
#include "spriteai/core/document/SpriteDocument.h"
#include "spriteai/core/document/Stroke.h"
#include "spriteai/core/document/Layer.h"

#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDir>
#include <algorithm>
#include <optional>

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
    // Find project root by looking for config directory
    QString projectRoot = QDir::currentPath();
    QDir searchDir(projectRoot);

    // Search up the directory tree for config/tools/default.tools.json
    int maxLevels = 5;
    while (maxLevels-- > 0) {
        if (QFile::exists(searchDir.filePath("config/tools/default.tools.json"))) {
            projectRoot = searchDir.absolutePath();
            break;
        }
        if (!searchDir.cdUp()) break;
    }

    m_hotReload->setProjectRoot(projectRoot);
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

    setupPixelSampler();
}

void CanvasWidget::setupPixelSampler()
{
    spriteai::core::tools::PixelSampler sampler;

    sampler.sampleAt = [this](float worldX, float worldY) -> std::optional<std::uint32_t> {
        if (!m_renderer || !m_view) return std::nullopt;

        float screenX, screenY;
        m_view->worldToScreen(worldX, worldY, screenX, screenY);

        int x = static_cast<int>(screenX);
        int y = static_cast<int>(screenY);

        if (x < 0 || x >= m_renderer->width() ||
            y < 0 || y >= m_renderer->height()) {
            return std::nullopt;
        }

        const auto* buf = m_renderer->rgba8Buffer();
        if (!buf) return std::nullopt;

        return buf[static_cast<size_t>(y) * static_cast<size_t>(m_renderer->width()) +
                   static_cast<size_t>(x)];
    };

    sampler.bufferWidth = width();
    sampler.bufferHeight = height();

    m_engine->toolContext().setPixelSampler(sampler);

    m_engine->toolContext().setColorCallback({
        [this](std::uint32_t rgba) {
            emit colorPicked(rgba);
        }
    });
}

CanvasWidget::~CanvasWidget() = default;

// ------------------------------------------------------------
// Tool Management
// ------------------------------------------------------------

void CanvasWidget::setTool(const QString& toolId)
{
    if (!m_hotReload) return;

    auto tool = m_hotReload->createToolById(toolId);
    if (tool) {
        if (auto* aiTool = dynamic_cast<spriteai::core::tools::builtin::AITool*>(tool.get())) {
            aiTool->setAIClient(m_engine->aiClient());
        }
        // Apply current foreground color to color-based tools
        std::uint32_t fgColor = m_engine->toolContext().foregroundColor();
        if (auto* brush = dynamic_cast<spriteai::core::tools::builtin::BrushTool*>(tool.get())) {
            brush->setColor(fgColor);
        }
        if (auto* pencil = dynamic_cast<spriteai::core::tools::builtin::PencilTool*>(tool.get())) {
            pencil->setColor(fgColor);
        }
        if (auto* fill = dynamic_cast<spriteai::core::tools::builtin::FillTool*>(tool.get())) {
            fill->setColor(fgColor);
        }
        m_engine->setActiveTool(std::move(tool));
        emit toolChanged(toolId);
        update();
    }
}

// ------------------------------------------------------------
// Edit Operations
// ------------------------------------------------------------

void CanvasWidget::undo()
{
    if (m_engine) {
        m_engine->undo();
        update();
    }
}

void CanvasWidget::redo()
{
    if (m_engine) {
        m_engine->redo();
        update();
    }
}

// ------------------------------------------------------------
// View Operations
// ------------------------------------------------------------

void CanvasWidget::zoomIn()
{
    if (m_view) {
        auto& cam = m_view->camera();
        cam.zoom = std::clamp(cam.zoom * 1.25f, 0.1f, 20.0f);
        emit zoomChanged(cam.zoom);
        update();
    }
}

void CanvasWidget::zoomOut()
{
    if (m_view) {
        auto& cam = m_view->camera();
        cam.zoom = std::clamp(cam.zoom / 1.25f, 0.1f, 20.0f);
        emit zoomChanged(cam.zoom);
        update();
    }
}

void CanvasWidget::zoomFit()
{
    if (m_view) {
        auto& cam = m_view->camera();
        cam.zoom = 1.0f;
        cam.panX = 0.0f;
        cam.panY = 0.0f;
        emit zoomChanged(cam.zoom);
        update();
    }
}

void CanvasWidget::setZoom(float zoom)
{
    if (m_view) {
        auto& cam = m_view->camera();
        cam.zoom = std::clamp(zoom, 0.1f, 20.0f);
        emit zoomChanged(cam.zoom);
        update();
    }
}

float CanvasWidget::zoom() const
{
    if (m_view) {
        return m_view->camera().zoom;
    }
    return 1.0f;
}

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


void CanvasWidget::scheduleUpdate() {
    if (!m_updateScheduled) {
        m_updateScheduled = true;
        QMetaObject::invokeMethod(this, [this]() {
            m_updateScheduled = false;
            update();
        }, Qt::QueuedConnection);
    }
}

void CanvasWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton)
        sendPointer(true, e);
    scheduleUpdate();
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton)
        sendPointer(true, e);
    scheduleUpdate();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton)
        sendPointer(false, e);
    scheduleUpdate();
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
            // Apply current foreground color to color-based tools
            std::uint32_t fgColor = m_engine->toolContext().foregroundColor();
            if (auto* brush = dynamic_cast<spriteai::core::tools::builtin::BrushTool*>(tool.get())) {
                brush->setColor(fgColor);
            }
            if (auto* pencil = dynamic_cast<spriteai::core::tools::builtin::PencilTool*>(tool.get())) {
                pencil->setColor(fgColor);
            }
            if (auto* fill = dynamic_cast<spriteai::core::tools::builtin::FillTool*>(tool.get())) {
                fill->setColor(fgColor);
            }
            m_engine->setActiveTool(std::move(tool));
            update();
            return;
        }
    }

    QWidget::keyPressEvent(e);
}

// ------------------------------------------------------------
// Color Operations
// ------------------------------------------------------------

void CanvasWidget::setForegroundColor(const QColor& color)
{
    if (!m_engine) return;

    std::uint32_t rgba = (static_cast<std::uint32_t>(color.alpha()) << 24) |
                         (static_cast<std::uint32_t>(color.red()) << 16) |
                         (static_cast<std::uint32_t>(color.green()) << 8) |
                         static_cast<std::uint32_t>(color.blue());

    m_engine->toolContext().setForegroundColor(rgba);

    if (auto* tool = m_engine->activeTool()) {
        if (auto* brush = dynamic_cast<spriteai::core::tools::builtin::BrushTool*>(tool)) {
            brush->setColor(rgba);
        }
        if (auto* pencil = dynamic_cast<spriteai::core::tools::builtin::PencilTool*>(tool)) {
            pencil->setColor(rgba);
        }
        if (auto* fill = dynamic_cast<spriteai::core::tools::builtin::FillTool*>(tool)) {
            fill->setColor(rgba);
        }
    }
}

void CanvasWidget::setBackgroundColor(const QColor& color)
{
    if (!m_engine) return;

    std::uint32_t rgba = (static_cast<std::uint32_t>(color.alpha()) << 24) |
                         (static_cast<std::uint32_t>(color.red()) << 16) |
                         (static_cast<std::uint32_t>(color.green()) << 8) |
                         static_cast<std::uint32_t>(color.blue());

    m_engine->toolContext().setBackgroundColor(rgba);
}

// ------------------------------------------------------------
// Brush Settings
// ------------------------------------------------------------

void CanvasWidget::setBrushSize(int size)
{
    if (!m_engine) return;

    if (auto* tool = m_engine->activeTool()) {
        if (auto* brush = dynamic_cast<spriteai::core::tools::builtin::BrushTool*>(tool)) {
            brush->setWidth(static_cast<float>(size));
        }
    }
}

void CanvasWidget::setBrushOpacity(int opacity)
{
    if (!m_engine) return;

    std::uint32_t fgColor = m_engine->toolContext().foregroundColor();
    std::uint32_t newAlpha = static_cast<std::uint32_t>((opacity * 255) / 100);
    fgColor = (newAlpha << 24) | (fgColor & 0x00FFFFFF);
    m_engine->toolContext().setForegroundColor(fgColor);

    if (auto* tool = m_engine->activeTool()) {
        if (auto* brush = dynamic_cast<spriteai::core::tools::builtin::BrushTool*>(tool)) {
            brush->setColor(fgColor);
        }
    }
}

void CanvasWidget::setBrushSpacing(int spacing)
{
    if (!m_engine) return;

    if (auto* tool = m_engine->activeTool()) {
        if (auto* brush = dynamic_cast<spriteai::core::tools::builtin::BrushTool*>(tool)) {
            brush->setSpacing(static_cast<float>(spacing) / 100.0f);
        }
    }
}

// ------------------------------------------------------------
// Grid and Overlays
// ------------------------------------------------------------

void CanvasWidget::setGridEnabled(bool enabled)
{
    if (!m_engine) return;
    m_engine->grid().enabled = enabled;
    update();
}

void CanvasWidget::setSymmetryEnabled(bool enabled)
{
    if (!m_engine) return;
    m_engine->symmetry().enabled = enabled;
    m_engine->symmetry().drawAxisLine = enabled;
    update();
}

// ------------------------------------------------------------
// Edit Operations
// ------------------------------------------------------------

void CanvasWidget::selectAll()
{
    if (!m_engine) return;

    float minX = 0, minY = 0, maxX = 0, maxY = 0;
    bool first = true;

    for (const auto& stroke : m_engine->document().strokes()) {
        for (const auto& pt : stroke.points) {
            if (first) {
                minX = maxX = pt.x;
                minY = maxY = pt.y;
                first = false;
            } else {
                minX = std::min(minX, pt.x);
                maxX = std::max(maxX, pt.x);
                minY = std::min(minY, pt.y);
                maxY = std::max(maxY, pt.y);
            }
        }
    }

    if (!first) {
        spriteai::core::selection::Rect rect{minX - 1, minY - 1, maxX - minX + 2, maxY - minY + 2};
        m_engine->selection().setRect(rect);
    }
    update();
}

void CanvasWidget::deselectAll()
{
    if (!m_engine) return;
    m_engine->selection().clear();
    update();
}

void CanvasWidget::deleteSelection()
{
    if (!m_engine) return;
    if (!m_engine->selection().hasSelection()) return;

    auto& strokes = m_engine->document().mutableStrokes();
    const auto& sel = m_engine->selection();

    strokes.erase(
        std::remove_if(strokes.begin(), strokes.end(),
            [&sel](const spriteai::core::document::Stroke& s) {
                for (const auto& pt : s.points) {
                    if (sel.containsPoint(pt.x, pt.y)) {
                        return true;
                    }
                }
                return false;
            }),
        strokes.end()
    );

    m_engine->selection().clear();
    update();
}

// ------------------------------------------------------------
// Image Operations
// ------------------------------------------------------------

void CanvasWidget::flipHorizontal()
{
    if (!m_engine) return;

    float minX = 0, maxX = 0;
    bool first = true;

    auto& strokes = m_engine->document().mutableStrokes();

    for (const auto& stroke : strokes) {
        for (const auto& pt : stroke.points) {
            if (first) {
                minX = maxX = pt.x;
                first = false;
            } else {
                minX = std::min(minX, pt.x);
                maxX = std::max(maxX, pt.x);
            }
        }
    }

    float centerX = (minX + maxX) / 2.0f;

    for (auto& stroke : strokes) {
        for (auto& pt : stroke.points) {
            pt.x = 2.0f * centerX - pt.x;
        }
    }
    update();
}

void CanvasWidget::flipVertical()
{
    if (!m_engine) return;

    float minY = 0, maxY = 0;
    bool first = true;

    auto& strokes = m_engine->document().mutableStrokes();

    for (const auto& stroke : strokes) {
        for (const auto& pt : stroke.points) {
            if (first) {
                minY = maxY = pt.y;
                first = false;
            } else {
                minY = std::min(minY, pt.y);
                maxY = std::max(maxY, pt.y);
            }
        }
    }

    float centerY = (minY + maxY) / 2.0f;

    for (auto& stroke : strokes) {
        for (auto& pt : stroke.points) {
            pt.y = 2.0f * centerY - pt.y;
        }
    }
    update();
}

void CanvasWidget::rotate90CW()
{
    if (!m_engine) return;

    float minX = 0, maxX = 0, minY = 0, maxY = 0;
    bool first = true;

    auto& strokes = m_engine->document().mutableStrokes();

    for (const auto& stroke : strokes) {
        for (const auto& pt : stroke.points) {
            if (first) {
                minX = maxX = pt.x;
                minY = maxY = pt.y;
                first = false;
            } else {
                minX = std::min(minX, pt.x);
                maxX = std::max(maxX, pt.x);
                minY = std::min(minY, pt.y);
                maxY = std::max(maxY, pt.y);
            }
        }
    }

    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;

    for (auto& stroke : strokes) {
        for (auto& pt : stroke.points) {
            float dx = pt.x - centerX;
            float dy = pt.y - centerY;
            pt.x = centerX - dy;
            pt.y = centerY + dx;
        }
    }
    update();
}

void CanvasWidget::rotate90CCW()
{
    if (!m_engine) return;

    float minX = 0, maxX = 0, minY = 0, maxY = 0;
    bool first = true;

    auto& strokes = m_engine->document().mutableStrokes();

    for (const auto& stroke : strokes) {
        for (const auto& pt : stroke.points) {
            if (first) {
                minX = maxX = pt.x;
                minY = maxY = pt.y;
                first = false;
            } else {
                minX = std::min(minX, pt.x);
                maxX = std::max(maxX, pt.x);
                minY = std::min(minY, pt.y);
                maxY = std::max(maxY, pt.y);
            }
        }
    }

    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;

    for (auto& stroke : strokes) {
        for (auto& pt : stroke.points) {
            float dx = pt.x - centerX;
            float dy = pt.y - centerY;
            pt.x = centerX + dy;
            pt.y = centerY - dx;
        }
    }
    update();
}

void CanvasWidget::clearCanvas()
{
    if (!m_engine) return;
    m_engine->document().clear();
    m_engine->selection().clear();
    update();
}

// ------------------------------------------------------------
// Layer Operations
// ------------------------------------------------------------

int CanvasWidget::addLayer()
{
    if (!m_engine) return -1;
    int index = m_engine->document().addLayer();
    emit layerCountChanged(m_engine->document().layerCount());
    emit activeLayerChanged(index);
    update();
    return index;
}

void CanvasWidget::deleteLayer(int index)
{
    if (!m_engine) return;
    m_engine->document().deleteLayer(index);
    emit layerCountChanged(m_engine->document().layerCount());
    emit activeLayerChanged(m_engine->document().activeLayerIndex());
    update();
}

void CanvasWidget::duplicateLayer(int index)
{
    if (!m_engine) return;
    m_engine->document().duplicateLayer(index);
    emit layerCountChanged(m_engine->document().layerCount());
    emit activeLayerChanged(m_engine->document().activeLayerIndex());
    update();
}

void CanvasWidget::moveLayer(int from, int to)
{
    if (!m_engine) return;
    m_engine->document().moveLayer(from, to);
    emit activeLayerChanged(m_engine->document().activeLayerIndex());
    update();
}

void CanvasWidget::setActiveLayer(int index)
{
    if (!m_engine) return;
    m_engine->document().setActiveLayer(index);
    emit activeLayerChanged(index);
    update();
}

void CanvasWidget::setLayerOpacity(int index, int opacity)
{
    if (!m_engine) return;
    float opacityF = static_cast<float>(opacity) / 100.0f;
    m_engine->document().setLayerOpacity(index, opacityF);
    update();
}

void CanvasWidget::setLayerBlendMode(int index, const QString& mode)
{
    if (!m_engine) return;
    auto blendMode = spriteai::core::document::stringToBlendMode(mode.toStdString());
    m_engine->document().setLayerBlendMode(index, blendMode);
    update();
}

int CanvasWidget::layerCount() const
{
    if (!m_engine) return 0;
    return m_engine->document().layerCount();
}

int CanvasWidget::activeLayerIndex() const
{
    if (!m_engine) return 0;
    return m_engine->document().activeLayerIndex();
}

} // namespace spriteai::editor::app
