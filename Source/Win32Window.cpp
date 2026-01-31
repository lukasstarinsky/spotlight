#include "Window.hpp"

#include <windows.h>

#include "Input.hpp"

struct WindowState
{
    void* BitMap;
    HHOOK KeyboardHook;
    HBITMAP BitMapHandle;
    HBITMAP OldBitMapHandle;
    HWND WindowHandle;
    HDC DeviceContext;
    HDC MemoryContext;
};

Window::Window()
{
    mState = new WindowState();
    mX = GetSystemMetrics(SM_XVIRTUALSCREEN);
    mY = GetSystemMetrics(SM_YVIRTUALSCREEN);
    mWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    mHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    mState->WindowHandle = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT, "STATIC", nullptr, WS_POPUP,
        mX, mY, mWidth, mHeight,
        nullptr, nullptr, nullptr, nullptr
    );
    Ensure(mState->WindowHandle != nullptr, "Failed to create window.");

    mState->DeviceContext = GetDC(nullptr);
    Ensure(mState->DeviceContext != nullptr, "Failed to get device context.");

    mState->MemoryContext = CreateCompatibleDC(mState->DeviceContext);
    Ensure(mState->MemoryContext != nullptr, "Failed to create compatible device context.");

    BITMAPINFO bitMapInfo = {};
    bitMapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitMapInfo.bmiHeader.biWidth = mWidth;
    bitMapInfo.bmiHeader.biHeight = -mHeight;
    bitMapInfo.bmiHeader.biPlanes = 1;
    bitMapInfo.bmiHeader.biBitCount = 32;
    bitMapInfo.bmiHeader.biCompression = BI_RGB;

    mState->BitMapHandle = CreateDIBSection(
        mState->MemoryContext,
        &bitMapInfo,
        DIB_RGB_COLORS,
        &mState->BitMap,
        nullptr,
        0
    );
    Ensure(mState->BitMapHandle != nullptr, "Failed to create DIB section.");

    auto oldBitmap = SelectObject(mState->MemoryContext, mState->BitMapHandle);
    Ensure(oldBitmap != nullptr, "Failed to select bitmap into device context.");

    mState->KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, [](i32 nCode, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        if (nCode == HC_ACTION)
        {
            const auto* kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
            switch (wParam)
            {
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                case WM_KEYUP:
                case WM_SYSKEYUP:
                {
                    bool isKeyDown = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
                    Input::SetKeyState(static_cast<Key>(kb->vkCode), isKeyDown);
                    break;
                }
            }
        }

        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    },
    nullptr, 0);
    Ensure(mState->KeyboardHook != nullptr, "Failed to set keyboard hook.");

    ShowWindow(mState->WindowHandle, SW_SHOW);
    UpdateWindow(mState->WindowHandle);
}

Window::~Window()
{
    UnhookWindowsHookEx(mState->KeyboardHook);
    SelectObject(mState->MemoryContext, mState->OldBitMapHandle);
    DeleteObject(mState->BitMapHandle);
    DeleteDC(mState->MemoryContext);
    ReleaseDC(nullptr, mState->DeviceContext);
    DestroyWindow(mState->WindowHandle);
}

auto Window::GetBitMap() const -> void*
{
    return mState->BitMap;
}

auto Window::GetWidth() const -> i32
{
    return mWidth;
}

auto Window::GetHeight() const -> i32
{
    return mHeight;
}

auto Window::GetX() const -> i32
{
    return mX;
}

auto Window::GetY() const -> i32
{
    return mY;
}

auto Window::PollEvents() -> bool
{
    MSG msg {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

auto Window::GetMousePos() const -> Vec2
{
    Vec2 mousePos;
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    mousePos.X = static_cast<i32>(cursorPos.x - mX);
    mousePos.Y = static_cast<i32>(cursorPos.y - mY);
    return mousePos;
}

void Window::UpdateScreen() const
{
    BLENDFUNCTION blendFunction = {};
    blendFunction.BlendOp = AC_SRC_OVER;
    blendFunction.BlendFlags = 0;
    blendFunction.SourceConstantAlpha = 255;
    blendFunction.AlphaFormat = AC_SRC_ALPHA;

    POINT srcPos = { 0, 0 };
    SIZE size = { mWidth, mHeight };
    POINT destPos = { mX, mY };

    UpdateLayeredWindow(
        mState->WindowHandle,
        mState->DeviceContext,
        &destPos,
        &size,
        mState->MemoryContext,
        &srcPos,
        0,
        &blendFunction,
        ULW_ALPHA
    );
}