#pragma once

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void resize(int width, int height) = 0;
    virtual void renderFrame() = 0;
};
