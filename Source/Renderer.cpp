#include "Renderer.hpp"

Renderer::Renderer(void* pixels, i32 width, i32 height)
    : mCanvas(static_cast<Color*>(pixels))
    , mWidth(width)
    , mHeight(height)
{
}

void Renderer::ClearColor(const Color& color)
{
    for (i32 y = 0; y < mHeight; ++y)
    {
        for (i32 x = 0; x < mWidth; ++x)
        {
            mCanvas[x + y * mWidth] = color;
        }
    }
}

void Renderer::DrawPixel(i32 x, i32 y, const Color& color)
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
        return;

    mCanvas[x + y * mWidth] = color;
}

void Renderer::DrawRectangle(i32 x, i32 y, i32 width, i32 height, const Color& color, bool outline, const Color& outlineColor)
{
    for (int col = 0; col <= width; ++col)
    {
        for (int row = 0; row <= height; ++row)
        {
            DrawPixel(x + col, y + row, color);
            if (outline)
            {
                if (col == 0 || col == width || row == 0 || row == height)
                {
                    DrawPixel(x + col, y + row, outlineColor);
                }
            }
        }
    }
}