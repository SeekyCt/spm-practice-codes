#include <common.h>
#include <spm/evtmgr_cmd.h>
#include <spm/romfont.h>
#include <spm/relmgr.h>
#include <spm/spmario.h>
#include <wii/DEMOInit.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <wii/os.h>
#include <wii/vi.h>
#include <msl/string.h>

#include "mod_ui_base/colours.h"
#include "mod.h"
#include "patch.h"
#include "util.h"

namespace mod {

// Romfont changed significantly in the Korean version so the exception handler won't work there currently
#ifndef SPM_KR0

extern "C" {
    bool inOSPanic = false;
    char exceptionWorkingText[256];

    void OSPanicForwarder();
    void exceptionOSReportForwarder();
}

static bool inException = false;
static char exceptionText[4096];
static u32 head = 0;
static void (*__OSUnhandledExceptionReal)(s32 p1, s32 p2, s32 p3, s32 p4);
static spm::evtmgr::EvtScriptCode * lastScript = nullptr;
static s32 (*evtmgrCmdReal)(spm::evtmgr::EvtEntry * entry) = nullptr;

#define SCREEN_TOP 228.0f
#define SCREEN_BOTTOM -228.0f
#define TITLE_Y 195.0f
#define TEXT_TOP (TITLE_Y - (LINE_HEIGHT * 2))
#define TEXT_BOTTOM (SCREEN_BOTTOM - LINE_HEIGHT)
#define TEXT_LEFT -300.0f
#define LINE_HEIGHT 15.0f

static wii::gx::GXColor titleColour {0xff, 0x20, 0x20, 0xff};

static void drawTitle(f32 scale)
{
    spm::romfont::romFontPrintGX(TEXT_LEFT, TITLE_Y, scale, &titleColour,
                                 "Exception - " MOD_VERSION " - %s Revision %d",
                                 getGameRegion(), getGameRevision());
    spm::romfont::romFontPrintGX(TEXT_LEFT, TITLE_Y - LINE_HEIGHT, scale, &titleColour,
                                 "Last Evt %x - relF %x - mod.rel %x", (u32) lastScript,
                                 (u32) spm::relmgr::relmgr_wp->relFile, (u32) getModRelLoadAddr());
}

static void draw(char * msg, f32 yShift, f32 scale)
{
    char * p = msg;
    bool done = false;
    const f32 x = TEXT_LEFT;
    f32 y = 200.0f - yShift;
    while (!done)
    {
        // Find end of line
        char * q = p;
        while ((*q != '\n') && (*q != '\0'))
            q++;
        
        // Split line into its own string temporarily
        done = *q == '\0';
        *q = '\0';
        
        // Draw line if on screen
        if ((y >= TEXT_BOTTOM) && (y <= TEXT_TOP))
            spm::romfont::romFontPrintGX(x, y, scale, &colours::white, p);

        // Move to next line
        y -= LINE_HEIGHT;
        p = q + 1;

        // Restore string
        if (!done)
            *q = '\n';
    }
}

static f32 getBottomY(char * msg)
{
    // Count newlines
    int n = 0;
    for (int i = 0; msg[i]; i++)
    {
        if (msg[i] == '\n')
            n++;
    }

    // Calculate
    return TEXT_TOP - (LINE_HEIGHT * n);
}

extern "C" void exceptionMessageHandler(char * msg)
{
    // spm::wpadmgr::wpadAllRumbleOff(0);
    // spm::spmario_snd::spsndExit();

    // Stop all other threads
    wii::os::OSThread * p = wii::os::currentThread->prev;
    while (p != nullptr)
    {
        wii::os::OSSuspendThread(p);
        p = p->prev;
    }
    p = wii::os::currentThread->next;
    while (p != nullptr)
    {
        wii::os::OSSuspendThread(p);
        p = p->next;
    }

    // Print to OSReport
    wii::os::OSReport("%s\n", msg);

    const f32 topY = 50.0f;
    f32 bottomY = getBottomY(msg);
    f32 yShift = topY;
    f32 delta = bottomY <= SCREEN_BOTTOM ? 1.0f : 0.0f;
    f32 scale = gIsDolphin ? 0.7f : 0.55f; // dolphin uses a custom font for copyright reasons
    while (true)
    {
        // Check if power button was pressed
        if (spm::spmario::spmario_doShutdown)
        {
            wii::vi::VISetBlack(1);
            wii::vi::VIFlush();
            wii::vi::VIWaitForRetrace();
            wii::vi::VIWaitForRetrace();
            wii::vi::VIWaitForRetrace();
            wii::os::OSShutdownSystem();
            while (true) {};
        }

        // Start frame
        wii::mtx::Mtx44 mtx;
        wii::DEMOInit::DEMOBeforeRender();
        wii::mtx::C_MTXOrtho(&mtx, SCREEN_TOP, SCREEN_BOTTOM, -304.0f, 304.0f, 1.0f, 1000.0f);
        wii::gx::GXSetProjection(&mtx, 1);

        // Draw game & mod version header
        drawTitle(scale);

        // Render main text
        draw(msg, yShift, scale);

        // Scroll for next frame
        if ((yShift >= topY) || (yShift <= bottomY))
            delta *= -1.0f;
        yShift += delta;

        // End frame
        wii::DEMOInit::DEMODoneRender();
    }
}

extern "C" void exceptionOSReport(const char * msg)
{
    // Print to OSReport
    wii::os::OSReport(msg);

    // Store message to be drawn to screen
    size_t len = msl::string::strlen(msg);
    if ((head + len) >= sizeof(exceptionText))
        return;    
    msl::string::strcpy(exceptionText + head, msg);
    head += len;
}

void exceptionDraw()
{
    exceptionMessageHandler(exceptionText);
}

void checkExceptionFlags()
{
    if (inException)
    {
        wii::os::OSReport("WARNING: Exception handler has crashed!\n");
        while (true) {};
    }

    if (inOSPanic)
        exceptionOSReport("WARNING: OSPanic handler has crashed!\n");
}

void exceptionPatch()
{   
    // OSPanic
    writeBranch(wii::os::OSPanic, 0x130, OSPanicForwarder); 

    // __OSUnhandledException
    writeBranchLink(wii::os::__OSUnhandledException, 0x50,  exceptionOSReportForwarder);
    writeBranchLink(wii::os::__OSUnhandledException, 0x1b0, exceptionOSReportForwarder);
    writeBranchLink(wii::os::__OSUnhandledException, 0x1bc, exceptionOSReportForwarder);
    writeBranchLink(wii::os::__OSUnhandledException, 0x1d8, exceptionOSReportForwarder);    
    writeBranchLink(wii::os::__OSUnhandledException, 0x1ec, exceptionOSReportForwarder);
    writeBranchLink(wii::os::__OSUnhandledException, 0x220, exceptionOSReportForwarder);
    writeBranchLink(wii::os::__OSUnhandledException, 0x234, exceptionOSReportForwarder);
    writeBranchLink(wii::os::__OSUnhandledException, 0x24c, exceptionOSReportForwarder);
    writeBranchLink(wii::os::__OSUnhandledException, 0x264, exceptionOSReportForwarder);
    writeBranchLink(wii::os::__OSUnhandledException, 0x274, exceptionOSReportForwarder);
    writeBranchLink(wii::os::__OSUnhandledException, 0x28c, exceptionOSReportForwarder);
    writeBranchLink(wii::os::__OSUnhandledException, 0x2a0, exceptionOSReportForwarder);
    writeBranchLink(wii::os::__OSUnhandledException, 0x2b4, exceptionOSReportForwarder);
    writeBranchLink(wii::os::__OSUnhandledException, 0x2d0, exceptionOSReportForwarder);
    writeBranch(wii::os::__OSUnhandledException, 0x2d4, exceptionDraw);
    __OSUnhandledExceptionReal = patch::hookFunction(wii::os::__OSUnhandledException,
        [](s32 p1, s32 p2, s32 p3, s32 p4)
        {
            checkExceptionFlags();
            inException = true;
            __OSUnhandledExceptionReal(p1, p2, p3, p4);
        }
    );
    wii::os::__OSUnhandledException_msg1[73] = '\n';
    wii::os::__OSUnhandledException_msg2[75] = '\n';
    wii::os::__OSUnhandledException_msg3[72] = '\n';

    // OSDumpContext
    writeBranchLink(wii::os::OSDumpContext, 0x2c, exceptionOSReportForwarder);
    writeBranchLink(wii::os::OSDumpContext, 0x58, exceptionOSReportForwarder);
    writeBranchLink(wii::os::OSDumpContext, 0x7c, exceptionOSReportForwarder);
    writeBranchLink(wii::os::OSDumpContext, 0x90, exceptionOSReportForwarder);
    writeBranchLink(wii::os::OSDumpContext, 0x9c, exceptionOSReportForwarder);
    writeBranchLink(wii::os::OSDumpContext, 0xc0, exceptionOSReportForwarder);
    writeBranchLink(wii::os::OSDumpContext, 0x120, exceptionOSReportForwarder);
    writeBranchLink(wii::os::OSDumpContext, 0x158, exceptionOSReportForwarder);
    writeBranchLink(wii::os::OSDumpContext, 0x174, exceptionOSReportForwarder);
    writeBranchLink(wii::os::OSDumpContext, 0x1ac, exceptionOSReportForwarder);
    writeBranchLink(wii::os::OSDumpContext, 0x1fc, exceptionOSReportForwarder);
    writeBranchLink(wii::os::OSDumpContext, 0x220, exceptionOSReportForwarder);

    // Track last script
    evtmgrCmdReal = patch::hookFunction(spm::evtmgr_cmd::evtmgrCmd,
        [](spm::evtmgr::EvtEntry * entry)
        {
            lastScript = entry->scriptStart;
            return evtmgrCmdReal(entry);
        }
    );
}

#else

void exceptionPatch()
{

}

#endif

}
