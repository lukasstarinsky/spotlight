#pragma once

#include "Common.hpp"

struct Color
{
    u8 Blue;
    u8 Green;
    u8 Red;
    u8 Alpha;
};

#define RGBA_TO_COLOR(rgba) Color{ \
    static_cast<u8>(((rgba) >>  8) & 0xFF), \
    static_cast<u8>(((rgba) >> 16) & 0xFF), \
    static_cast<u8>(((rgba) >> 24) & 0xFF), \
    static_cast<u8>(((rgba) >>  0) & 0xFF)  \
}

class Renderer
{
public:
    Renderer(void* pixels, i32 width, i32 height);

    void ClearColor(const Color& color);
    void DrawPixel(i32 x, i32 y, const Color& color);
    void DrawRectangle(i32 x, i32 y, i32 width, i32 height, const Color& color, bool outline = false, const Color& outlineColor = { 0, 0, 0, 255 });
private:
    Color* mCanvas;
    i32 mWidth;
    i32 mHeight;
};