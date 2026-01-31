#include <print>
#include <windows.h>
#include <stdexcept>

#define Ensure(predicate, message) if (!(predicate)) throw std::runtime_error(message)

struct Pixel
{
    uint8_t Blue;
    uint8_t Green;
    uint8_t Red;
    uint8_t Alpha;
};

#define RGBA_TO_PIXEL(rgba) Pixel{ \
    static_cast<uint8_t>(((rgba) >>  8) & 0xFF), \
    static_cast<uint8_t>(((rgba) >> 16) & 0xFF), \
    static_cast<uint8_t>(((rgba) >> 24) & 0xFF), \
    static_cast<uint8_t>(((rgba) >>  0) & 0xFF)  \
}

void ResetColor(Pixel* pixels, int width, int height, Pixel color)
{
    for (int i = 0; i < width * height; ++i)
    {
        pixels[i] = color;
    }
}

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

    SetWindowsHook(WH_KEYBOARD_LL, LowLevelKeyboardProc);

    ShowWindow(windowHandle, SW_SHOW);
    UpdateWindow(windowHandle);

    BITMAPINFO bitMapInfo = {};
    bitMapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitMapInfo.bmiHeader.biWidth = w;
    bitMapInfo.bmiHeader.biHeight = -h;
    bitMapInfo.bmiHeader.biPlanes = 1;
    bitMapInfo.bmiHeader.biBitCount = 32;
    bitMapInfo.bmiHeader.biCompression = BI_RGB;

    void* bits = nullptr;
    auto bitmapHandle = CreateDIBSection(
        memContext,
        &bitMapInfo,
        DIB_RGB_COLORS,
        &bits,
        nullptr,
        0
    );
    Ensure(bitmapHandle != nullptr, "Failed to create DIB section.");

    auto oldBitmap = SelectObject(memContext, bitmapHandle);
    Ensure(oldBitmap != nullptr, "Failed to select bitmap into device context.");

    auto* pixels = static_cast<Pixel*>(bits);

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

        ResetColor(pixels, w, h, RGBA_TO_PIXEL(0x000000A0));

        POINT cursorPos;
        GetCursorPos(&cursorPos);
        auto cursorX = cursorPos.x - x;
        auto cursorY = cursorPos.y - y;

        const auto radius = 100;
        for (int col = -radius; col <= radius; ++col)
        {
            for (int row = -radius; row <= radius; ++row)
            {
                auto index = cursorX + col + (cursorY + row) * w;
                if (index < 0 || index >= w * h)
                    continue;

                if (col == -radius || col == radius || row == -radius || row == radius)
                {
                    pixels[cursorX + col + (cursorY + row) * w] = RGBA_TO_PIXEL(0xFFFFFFFF);
                }
                else
                {
                    pixels[cursorX + col + (cursorY + row) * w] = RGBA_TO_PIXEL(0x00000000);
                }
            }
        }

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