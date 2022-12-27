/*
    APWindow
    Command-based window to preview an animpose model
*/

#pragma once

#include <common.h>
#include <wii/mtx.h>

#include "mod_ui_base/window.h"
#include "pyconsole.h"

#ifdef PYCONSOLE_PROTOTYPE

namespace mod {

class APWindow final : public Window
{
protected:
    s32 mApId;
    const char * mApAnimName;
    wii::mtx::Vec3 mApPos;
    wii::mtx::Vec3 mApScale;
    bool mApRestartAnim;

    static APWindow * sInstance;

    void release();
    void announceAnim(const char * name, u32 idx, u32 count);
    virtual void preDisp() override;
    virtual void disp() override;
    APWindow();

    PyConsoleErr cmd_start(s32 argc, const char ** argv);
    PyConsoleErr cmd_stop(s32 argc, const char ** argv);
    PyConsoleErr cmd_pos(s32 argc, const char ** argv);
    PyConsoleErr cmd_scale(s32 argc, const char ** argv);
    PyConsoleErr cmd_anim(s32 argc, const char ** argv);
    PyConsoleErr cmd_anim_idx(s32 argc, const char ** argv);

public:
    static void init();
};

}
#endif
