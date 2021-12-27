/*
    ConsoleWindow
    Window that displays an on-screen console
*/

#pragma once

#include "mod_ui_base/window.h"

#include <types.h>
#include <wii/stdio.h>
#include <wii/types.h>

namespace mod {

typedef void (ConsoleFreeCallback)(const char * line);

class ConsoleWindow final : public Window
{
protected:
    struct ConsoleLine
    {
        const char * line;
        wii::RGBA colour;
        u32 lifetime;
        ConsoleLine * next;
        ConsoleFreeCallback * freeCallback;
    };
    ConsoleLine * mLines;

    static ConsoleWindow * sInstance;

    virtual void disp() override;
    void _push(const char * text, const wii::RGBA * colour, ConsoleFreeCallback * cb);
    ConsoleWindow();
    
public:
    // Window::mPosX/Y are used for bottom left corner of window
    f32 mScale;
    u32 mFadeThreshhold;
    u32 mLineLifetime;

    #define CONSOLE_PUSH_FMT(format, ...)                                                \
        do                                                                               \
        {                                                                                \
            size_t _fmt_size = wii::stdio::snprintf(nullptr, 0, format, __VA_ARGS__);    \
            char * _fmt_str = new char[_fmt_size + 1];                                   \
            wii::stdio::sprintf(_fmt_str, format, __VA_ARGS__);                          \
            mod::ConsoleWindow::push(_fmt_str, nullptr, mod::ConsoleWindow::autoFreeCb); \
        } while (0)

    #define CONSOLE_PUSH_FMT_CLR(colour, format, ...)                                   \
        do                                                                              \
        {                                                                               \
            size_t _fmt_size = wii::stdio::snprintf(nullptr, 0, format, __VA_ARGS__);   \
            char * _fmt_str = new char[_fmt_size + 1];                                  \
            wii::stdio::sprintf(_fmt_str, format, __VA_ARGS__);                         \
            mod::ConsoleWindow::push(_fmt_str, colour, mod::ConsoleWindow::autoFreeCb); \
        } while (0)

    static void push(const char * text, const wii::RGBA * colour = nullptr, ConsoleFreeCallback * cb = nullptr);
    static void pushClone(const char * text, const wii::RGBA * colour = nullptr);

    static void autoFreeCb(const char * line);
    static void init();
};

}
