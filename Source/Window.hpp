#pragma once

#include "Common.hpp"

struct WindowState;

class Window final
{
public:
    Window();
    ~Window();

    auto GetBitMap() const -> void*;
    auto GetWidth() const -> i32;
    auto GetHeight() const -> i32;
    auto GetX() const -> i32;
    auto GetY() const -> i32;
    auto PollEvents() -> bool;
    auto GetMousePos() const -> Vec2;
    void UpdateScreen() const;
private:
    i32 mWidth {};
    i32 mHeight {};
    i32 mX {};
    i32 mY {};
    WindowState* mState {};
};