#pragma once
#include <cstdint>

namespace spriteai::core::document { class SpriteDocument; }
namespace spriteai::engine { class EngineContext; }

namespace spriteai::canvas {

struct Camera2D {
    float zoom = 1.0f;
    float panX = 0.0f;
    float panY = 0.0f;
};

class CanvasView {
public:
    explicit CanvasView(spriteai::core::document::SpriteDocument* doc, spriteai::engine::EngineContext* ctx=nullptr)
        : m_doc(doc), m_ctx(ctx) {}

    spriteai::engine::EngineContext* context() const { return m_ctx; }
    void setContext(spriteai::engine::EngineContext* ctx) { m_ctx = ctx; }

    spriteai::core::document::SpriteDocument* document() const { return m_doc; }

    Camera2D& camera() { return m_camera; }
    const Camera2D& camera() const { return m_camera; }

    // Screen->World transform (minimal). Editor can override by feeding proper camera.
    void screenToWorld(float sx, float sy, float& wx, float& wy) const {
        wx = (sx / m_camera.zoom) + m_camera.panX;
        wy = (sy / m_camera.zoom) + m_camera.panY;
    }

    // World->Screen
    void worldToScreen(float wx, float wy, float& sx, float& sy) const {
        sx = (wx - m_camera.panX) * m_camera.zoom;
        sy = (wy - m_camera.panY) * m_camera.zoom;
    }

private:
    spriteai::core::document::SpriteDocument* m_doc = nullptr;
    spriteai::engine::EngineContext* m_ctx = nullptr;
    Camera2D m_camera{};
};

} // namespace spriteai::canvas
