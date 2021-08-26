#include "apwindow.h"
#include "patch.h"
#include "pyconsole.h"
#include "util.h"

#include <types.h>
#include <spm/animdrv.h>
#include <spm/camdrv.h>
#include <wii/mtx.h>
#include <wii/string.h>
#include <wii/stdio.h>

namespace mod {

APWindow * APWindow::sInstance = nullptr;

void APWindow::release()
{
    if (mApId != -1)
    {
        spm::animdrv::animPoseRelease(mApId);
        mApId = -1;
    }
    mApPos = {0.0f, 0.0f, 0.0f};
    mApScale = {1.0f, 1.0f, 1.0f};
}

PyConsoleErr APWindow::cmd_start(s32 argc, const char ** argv)
{
    if (argc != 1)
        return PyConsoleErr::BAD_ARG_COUNT;
    
    const char * name = argv[0];

    if (wii::string::strlen(name) >= 32)
        return PyConsoleErr::BAD_ARGS;
    
    char path[32 + 3];
    wii::stdio::sprintf(path, "a/%s", name);
    if (!fileExists(path))
        return PyConsoleErr::BAD_ARGS;
    
    release();

    mApId = spm::animdrv::animPoseEntry(name, -1);

    return PyConsoleErr::NONE;
}

PyConsoleErr APWindow::cmd_stop(s32 argc, const char ** argv)
{
    if (argc != 0)
        return PyConsoleErr::BAD_ARG_COUNT;
    
    release();

    return PyConsoleErr::NONE;
}

PyConsoleErr APWindow::cmd_pos(s32 argc, const char ** argv)
{
    if (argc != 3)
        return PyConsoleErr::BAD_ARG_COUNT;
    
    f32 coords[3];
    for (s32 i = 0; i < 3; i++)
    {
        if (wii::stdio::sscanf(argv[i], "%f", &coords[i]) != 1)
            return PyConsoleErr::BAD_ARGS;
    }
    
    mApPos = {coords[0], coords[1], coords[2]};

    return PyConsoleErr::NONE;
}

PyConsoleErr APWindow::cmd_scale(s32 argc, const char ** argv)
{
    if (argc != 3)
        return PyConsoleErr::BAD_ARG_COUNT;

    f32 scale[3];
    for (s32 i = 0; i < 3; i++)
    {
        if (wii::stdio::sscanf(argv[i], "%f", &scale[i]) != 1)
            return PyConsoleErr::BAD_ARGS;
    }
    
    mApScale = {scale[0], scale[1], scale[2]};

    return PyConsoleErr::NONE;
}

void APWindow::preDisp()
{
    if (mApId == -1)
        return;
    
    spm::animdrv::animPoseMain(mApId);
}

void APWindow::disp()
{
    if (mApId == -1)
        return;

    wii::Mtx34 mtx;
    wii::mtx::PSMTXScale(&mtx, mApScale.x, mApScale.y, mApScale.z);
    wii::mtx::PSMTXTransApply(&mtx, &mtx, mApPos.x, mApPos.y, mApPos.z);

    spm::animdrv::animPoseDrawMtx(mApId, &mtx, 1, 0.0f, 1.0f);
    spm::animdrv::animPoseDrawMtx(mApId, &mtx, 2, 0.0f, 1.0f);
    spm::animdrv::animPoseDrawMtx(mApId, &mtx, 3, 0.0f, 1.0f);
}

APWindow::APWindow()
{
    mCamera = spm::camdrv::CAM_3D;
    mApPos = {0.0f, 0.0f, 0.0f};
    mApScale = {1.0f, 1.0f, 1.0f};
    mApId = -1;
}

#define REGISTER_CMD(name, method)                             \
    PyCommand::add(name,                                       \
        [](s32 argc, const char ** argv)                       \
        {                                                      \
            return APWindow::sInstance->method(argc, argv);    \
        }                                                      \
    )

static void (*animPoseAutoReleaseReal)(s32 releaseType);
void APWindow::init()
{
    REGISTER_CMD("ap_start", cmd_start);
    REGISTER_CMD("ap_pos", cmd_pos);
    REGISTER_CMD("ap_scale", cmd_scale);

    sInstance = new APWindow();

    animPoseAutoReleaseReal = patch::hookFunction(spm::animdrv::animPoseAutoRelease,
        [](s32 releaseType)
        {
            sInstance->release();
            animPoseAutoReleaseReal(releaseType);
        }
    );

}

}
