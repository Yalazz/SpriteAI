#pragma once
#include <cstdint>

namespace spriteai::canvas { class CanvasView; }

namespace spriteai::engine::renderer {

class ICanvasRenderer {
public:
    virtual ~ICanvasRenderer() = default;

    virtual void setCanvasSize(int w, int h) = 0;
    virtual void clear(std::uint32_t rgba) = 0;

    // Renders into an internal RGBA8 buffer.
    virtual void render(const spriteai::canvas::CanvasView& view) = 0;

    // Returns pointer to internal RGBA8 buffer (size = width * height).
    virtual const std::uint32_t* rgba8Buffer() const = 0;

    virtual int width() const = 0;
    virtual int height() const = 0;
};

} // namespace spriteai::engine::renderer
