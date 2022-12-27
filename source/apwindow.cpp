#include <common.h>
#include <spm/animdrv.h>
#include <spm/camdrv.h>
#include <wii/mtx.h>
#include <msl/stdio.h>
#include <msl/string.h>

#include "apwindow.h"
#include "consolewindow.h"
#include "patch.h"
#include "pyconsole.h"
#include "util.h"

#ifdef PYCONSOLE_PROTOTYPE

namespace mod {

APWindow * APWindow::sInstance = nullptr;

void APWindow::announceAnim(const char * name, u32 idx, u32 count)
{
    CONSOLE_PUSH_FMT("Playing %s (%d/%d)", name, idx, count - 1);
}

void APWindow::release()
{
    if (mApId != -1)
    {
        spm::animdrv::animPoseRelease(mApId);
        mApId = -1;
        mApAnimName = nullptr;
    }
}

PyConsoleErr APWindow::cmd_start(s32 argc, const char ** argv)
{
    if (argc != 1)
        return PyConsoleErr::BAD_ARG_COUNT;
    
    const char * name = argv[0];

    if (msl::string::strlen(name) >= 32)
        return PyConsoleErr::BAD_ARGS;
    
    char path[32 + 3];
    msl::stdio::sprintf(path, "a/%s", name);
    if (!fileExists(path))
        return PyConsoleErr::BAD_ARGS;
    
    release();

    mApId = spm::animdrv::animPoseEntry(name, 0);
    mApAnimName = nullptr;

    return PyConsoleErr::NONE;
}

PyConsoleErr APWindow::cmd_stop(s32 argc, const char ** argv)
{
    (void) argv;

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
        if (msl::stdio::sscanf(argv[i], "%f", &coords[i]) != 1)
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
        if (msl::stdio::sscanf(argv[i], "%f", &scale[i]) != 1)
            return PyConsoleErr::BAD_ARGS;
    }
    
    mApScale = {scale[0], scale[1], scale[2]};

    return PyConsoleErr::NONE;
}

PyConsoleErr APWindow::cmd_anim(s32 argc, const char ** argv)
{
    if (argc != 1)
        return PyConsoleErr::BAD_ARG_COUNT;

    const char * name = argv[0];

    spm::animdrv::AnimationModelFileHeader * header = spm::animdrv::animPoseGetAnimBaseDataPtr(mApId);    
    bool found = false;
    u32 i;
    for (i = 0; i < header->animCount; i++)
    {
        spm::animdrv::AnimationModelFileAnimTableEntry * anim = header->anims + i;
        if (msl::string::strcmp(anim->name, name) == 0)
        {
            found = true;
            break;
        }
    }
    if (!found)
        return PyConsoleErr::BAD_ARGS;
    
    if (mApAnimName != nullptr && msl::string::strcmp(mApAnimName, name) == 0)
        mApRestartAnim = true;
    mApAnimName = header->anims[i].name;

    announceAnim(mApAnimName, i, header->animCount);

    return PyConsoleErr::NONE;
}

PyConsoleErr APWindow::cmd_anim_idx(s32 argc, const char ** argv)
{
    if (argc != 1)
        return PyConsoleErr::BAD_ARG_COUNT;

    spm::animdrv::AnimationModelFileHeader * header = spm::animdrv::animPoseGetAnimBaseDataPtr(mApId);    

    u32 idx;
    if (msl::stdio::sscanf(argv[0], "%u", &idx) != 1 || idx >= header->animCount)
        return PyConsoleErr::BAD_ARGS;
    
    const char * name = header->anims[idx].name;
    if (mApAnimName != nullptr && msl::string::strcmp(mApAnimName, name) == 0)
        mApRestartAnim = true;
    mApAnimName = name;

    announceAnim(mApAnimName, idx, header->animCount);

    return PyConsoleErr::NONE;
}

void APWindow::preDisp()
{
    if (mApId == -1)
        return;
    
    spm::animdrv::animPoseSetAnim(mApId, mApAnimName, mApRestartAnim);
    if (mApRestartAnim)
        mApRestartAnim = false;
    spm::animdrv::animPoseMain(mApId);
}

void APWindow::disp()
{
    if (mApId == -1)
        return;

    wii::mtx::Mtx34 mtx;
    wii::mtx::PSMTXScale(&mtx, mApScale.x, mApScale.y, mApScale.z);
    wii::mtx::PSMTXTransApply(&mtx, &mtx, mApPos.x, mApPos.y, mApPos.z);

    spm::animdrv::animPoseDrawMtx(mApId, &mtx, 1, 0.0f, 1.0f);
    spm::animdrv::animPoseDrawMtx(mApId, &mtx, 2, 0.0f, 1.0f);
    spm::animdrv::animPoseDrawMtx(mApId, &mtx, 3, 0.0f, 1.0f);
}

APWindow::APWindow()
{
    mCamera = spm::camdrv::CAM_ID_3D;
    mApPos = {0.0f, 0.0f, 0.0f};
    mApScale = {1.0f, 1.0f, 1.0f};
    mApId = -1;
    mApAnimName = 0;
    mApRestartAnim = false;
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
    REGISTER_CMD("ap_stop", cmd_stop);
    REGISTER_CMD("ap_pos", cmd_pos);
    REGISTER_CMD("ap_scale", cmd_scale);
    REGISTER_CMD("ap_anim", cmd_anim);
    REGISTER_CMD("ap_anim_idx", cmd_anim_idx);

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
#endif
