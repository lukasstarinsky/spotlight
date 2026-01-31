#include <windows.h>

#include "Common.hpp"
#include "Renderer.hpp"

auto main() -> int
{
    int x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    std::println("Screen dimensions: x = {}, y = {}, w = {}, h = {}", x, y, w, h);

    auto windowHandle = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST, "STATIC", nullptr, WS_POPUP,
        x, y, w, h,
        nullptr, nullptr, nullptr, nullptr
    );
    Ensure(windowHandle != nullptr, "Failed to create window.");

    auto deviceContext = GetDC(nullptr);
    Ensure(deviceContext != nullptr, "Failed to get device context.");

    auto memContext = CreateCompatibleDC(deviceContext);
    Ensure(memContext != nullptr, "Failed to create compatible device context.");

    ShowWindow(windowHandle, SW_SHOW);
    UpdateWindow(windowHandle);

    BITMAPINFO bitMapInfo = {};
    bitMapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitMapInfo.bmiHeader.biWidth = w;
    bitMapInfo.bmiHeader.biHeight = -h;
    bitMapInfo.bmiHeader.biPlanes = 1;
    bitMapInfo.bmiHeader.biBitCount = 32;
    bitMapInfo.bmiHeader.biCompression = BI_RGB;

    void* bitmap = nullptr;
    auto bitmapHandle = CreateDIBSection(
        memContext,
        &bitMapInfo,
        DIB_RGB_COLORS,
        &bitmap,
        nullptr,
        0
    );
    Ensure(bitmapHandle != nullptr, "Failed to create DIB section.");

    auto oldBitmap = SelectObject(memContext, bitmapHandle);
    Ensure(oldBitmap != nullptr, "Failed to select bitmap into device context.");

    Renderer renderer(bitmap, w, h);

    BLENDFUNCTION blendFunction = {};
    blendFunction.BlendOp = AC_SRC_OVER;
    blendFunction.BlendFlags = 0;
    blendFunction.SourceConstantAlpha = 255;
    blendFunction.AlphaFormat = AC_SRC_ALPHA;

    POINT srcPos = { 0, 0 };
    SIZE size = { w, h };
    POINT destPos = { x, y };

    MSG msg {};
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT || (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE))
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        renderer.ClearColor(RGBA_TO_COLOR(0x000000A0));

        POINT cursorPos;
        GetCursorPos(&cursorPos);
        auto cursorX = cursorPos.x - x;
        auto cursorY = cursorPos.y - y;

        const auto radius = 100;
        renderer.DrawRectangle(cursorX - radius, cursorY - radius, radius * 2, radius * 2, RGBA_TO_COLOR(0x00000000), true, RGBA_TO_COLOR(0xFFFFFFFF));

        UpdateLayeredWindow(
            windowHandle,
            deviceContext,
            &destPos,
            &size,
            memContext,
            &srcPos,
            0,
            &blendFunction,
            ULW_ALPHA
        );
    }

    SelectObject(memContext, oldBitmap);
    DeleteObject(bitmapHandle);
    DeleteDC(memContext);
    ReleaseDC(nullptr, deviceContext);
    DestroyWindow(windowHandle);

    return 0;
}