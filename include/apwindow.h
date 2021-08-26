/*
    APWindow
    Command-based window to preview an animpose model
*/

#pragma once

#include "mod_ui_base/window.h"
#include "pyconsole.h"

#include <wii/types.h>

namespace mod {

class APWindow final : public Window
{
protected:
    s32 mApId;
    wii::Vec3 mApPos;
    wii::Vec3 mApScale;

    static APWindow * sInstance;

    void release();
    virtual void preDisp() override;
    virtual void disp() override;
    APWindow();

    PyConsoleErr cmd_start(s32 argc, const char ** argv);
    PyConsoleErr cmd_stop(s32 argc, const char ** argv);
    PyConsoleErr cmd_pos(s32 argc, const char ** argv);
    PyConsoleErr cmd_scale(s32 argc, const char ** argv);

public:
    static void init();
};

}
