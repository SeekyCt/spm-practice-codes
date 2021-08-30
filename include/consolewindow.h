/*
    ConsoleWindow
    Window that displays an on-screen console
*/

#pragma once

#include "mod_ui_base/window.h"

#include <types.h>
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
    void _push(const char * text, ConsoleFreeCallback * cb, const wii::RGBA * colour);
    ConsoleWindow();
    
public:
    // Window::mPosX/Y are used for bottom left corner of window
    f32 mScale;
    u32 mFadeThreshhold;
    u32 mLineLifetime;

    static void push(const char * text, ConsoleFreeCallback * cb = nullptr, const wii::RGBA * colour = nullptr);

    static void autoFreeCb(const char * line);
    static void init();
};

}
