#include "Input.hpp"

KeyState Input::mKeyStates[] = {};

void Input::SetKeyState(Key keycode, bool pressed)
{
    auto& state = mKeyStates[static_cast<i32>(keycode)];
    state.Pressed = pressed && !state.Down;
    state.Released = !pressed && state.Down;
    state.Down = pressed;
}

void Input::Clear()
{
    for (auto& state : mKeyStates)
    {
        state.Pressed = false;
        state.Released = false;
    }
}

auto Input::IsKeyPressed(Key keycode) -> bool
{
    auto index = static_cast<i32>(keycode);
    return mKeyStates[index].Down;
}

auto Input::IsKeyJustPressed(Key keycode) -> bool
{
    auto index = static_cast<i32>(keycode);
    return mKeyStates[index].Pressed;
}