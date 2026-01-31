#include "Renderer.hpp"
#include "Window.hpp"
#include "Input.hpp"

auto main() -> int
{
    bool shouldDraw = false;
    bool shouldClear = true;
    Window window;
    Renderer renderer(window.GetBitMap(), window.GetWidth(), window.GetHeight());

    while (true)
    {
        Input::Clear();

        if (!window.PollEvents())
            break;

        if (Input::IsKeyJustPressed(Key::F1))
        {
            if (shouldDraw)
            {
                shouldClear = true;
            }
            shouldDraw = !shouldDraw;
        }

        if (shouldClear)
        {
            renderer.ClearColor(RGBA_TO_COLOR(0x00000000));
            window.UpdateScreen();
            shouldClear = false;
        }
        else if (shouldDraw)
        {
            renderer.ClearColor(RGBA_TO_COLOR(0x000000AA));
            auto cursorPos = window.GetMousePos();
            const auto radius = 100;
            renderer.DrawRectangle(
                cursorPos.X - radius, cursorPos.Y - radius,
                radius * 2, radius * 2,
                RGBA_TO_COLOR(0x00000000),
                true, RGBA_TO_COLOR(0xFFFFFFFF)
            );
            window.UpdateScreen();
        }
    }

    return 0;
}