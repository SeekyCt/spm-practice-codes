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

class ConsoleWindow : public Window
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

    virtual void disp() override;
    
public:
    // Window::mPosX/Y are used for bottom left corner of window
    f32 mScale;
    u32 mFadeThreshhold;
    u32 mLineLifetime;

    static ConsoleWindow * sInstance;

    ConsoleWindow();
    void push(const char * text, ConsoleFreeCallback * cb = nullptr, const wii::RGBA * colour = nullptr);
};

}
