#pragma once

#include "Common.hpp"

enum class Key : uint16_t
{
    // Control keys
    Backspace   = 0x08,
    Tab         = 0x09,
    Clear       = 0x0C,
    Enter       = 0x0D,
    Shift       = 0x10,
    Control     = 0x11,
    Alt         = 0x12,
    Pause       = 0x13,
    CapsLock   = 0x14,
    Escape     = 0x1B,
    Space      = 0x20,

    PageUp      = 0x21,
    PageDown    = 0x22,
    End         = 0x23,
    Home        = 0x24,

    Left        = 0x25,
    Up          = 0x26,
    Right       = 0x27,
    Down        = 0x28,

    PrintScreen = 0x2C,
    Insert      = 0x2D,
    Delete      = 0x2E,

    // Digits
    D0 = 0x30,
    D1 = 0x31,
    D2 = 0x32,
    D3 = 0x33,
    D4 = 0x34,
    D5 = 0x35,
    D6 = 0x36,
    D7 = 0x37,
    D8 = 0x38,
    D9 = 0x39,

    // Letters
    A = 0x41, B = 0x42, C = 0x43, D = 0x44, E = 0x45,
    F = 0x46, G = 0x47, H = 0x48, I = 0x49, J = 0x4A,
    K = 0x4B, L = 0x4C, M = 0x4D, N = 0x4E, O = 0x4F,
    P = 0x50, Q = 0x51, R = 0x52, S = 0x53, T = 0x54,
    U = 0x55, V = 0x56, W = 0x57, X = 0x58, Y = 0x59,
    Z = 0x5A,

    // Windows keys
    LeftWin     = 0x5B,
    RightWin    = 0x5C,
    Apps        = 0x5D,

    // Numpad
    Num0        = 0x60,
    Num1        = 0x61,
    Num2        = 0x62,
    Num3        = 0x63,
    Num4        = 0x64,
    Num5        = 0x65,
    Num6        = 0x66,
    Num7        = 0x67,
    Num8        = 0x68,
    Num9        = 0x69,

    NumMultiply = 0x6A,
    NumAdd      = 0x6B,
    NumSeparator= 0x6C,
    NumSubtract = 0x6D,
    NumDecimal  = 0x6E,
    NumDivide   = 0x6F,

    // Function keys
    F1  = 0x70, F2  = 0x71, F3  = 0x72, F4  = 0x73,
    F5  = 0x74, F6  = 0x75, F7  = 0x76, F8  = 0x77,
    F9  = 0x78, F10 = 0x79, F11 = 0x7A, F12 = 0x7B,
    F13 = 0x7C, F14 = 0x7D, F15 = 0x7E, F16 = 0x7F,
    F17 = 0x80, F18 = 0x81, F19 = 0x82, F20 = 0x83,
    F21 = 0x84, F22 = 0x85, F23 = 0x86, F24 = 0x87,

    // Locks
    NumLock     = 0x90,
    ScrollLock  = 0x91,

    // Left / Right modifiers
    LeftShift   = 0xA0,
    RightShift  = 0xA1,
    LeftCtrl    = 0xA2,
    RightCtrl   = 0xA3,
    LeftAlt     = 0xA4,
    RightAlt    = 0xA5,

    // OEM (layout-dependent)
    Semicolon   = 0xBA,
    Plus        = 0xBB,
    Comma       = 0xBC,
    Minus       = 0xBD,
    Period      = 0xBE,
    Slash       = 0xBF,
    Tilde       = 0xC0,
    LBracket    = 0xDB,
    Backslash   = 0xDC,
    RBracket    = 0xDD,
    Quote       = 0xDE,
    Total
};

struct KeyState
{
    bool Down;
    bool Pressed;
    bool Released;
};

class Input
{
public:
    static void SetKeyState(Key keycode, bool pressed);

    static void Clear();
    static auto IsKeyPressed(Key keycode) -> bool;
    static auto IsKeyJustPressed(Key keycode) -> bool;
private:
    static KeyState mKeyStates[static_cast<i32>(Key::Total)];
};