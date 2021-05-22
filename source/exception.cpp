#include "mod.h"
#include "patch.h"

#include <types.h>
#include <spm/fontmgr.h>
#include <spm/romfont.h>
#include <spm/spmario.h>
#include <spm/system.h>
#include <wii/DEMOInit.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <wii/OSError.h>
#include <wii/OSReset.h>
#include <wii/OSThread.h>
#include <wii/string.h>
#include <wii/vi.h>

namespace mod {

extern "C" {
    bool inOSPanic = false;
    char exceptionWorkingText[256];

    void OSPanicForwarder();
    void exceptionOSReportForwarder();
}

// Romfont changed significantly in the Korean version so the exception handler won't work there currently
#ifndef SPM_KR0

static bool inException = false;
static char exceptionText[4096];
static u32 head = 0;
static void (*__OSUnhandledExceptionReal)(int p1, int p2, int p3, int p4);

#define SCREEN_TOP 228.0f
#define SCREEN_BOTTOM -228.0f
#define LINE_HEIGHT 15.0f

static void draw(char * msg, f32 yShift, f32 scale)
{
    char * p = msg;
    bool done = false;
    const f32 x = -300.0f;
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
        if ((y >= (SCREEN_BOTTOM - LINE_HEIGHT)) && (y <= SCREEN_TOP + LINE_HEIGHT))
            spm::romfont::romFontPrintGX(x, y, scale, {0xff, 0xff, 0xff, 0xff}, p);

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
    return SCREEN_TOP - (LINE_HEIGHT * n);
}

extern "C" void exceptionMessageHandler(char * msg)
{
    // spm::wpadmgr::wpadAllRumbleOff(0);
    // spm::spmario_snd::spsndExit();

    // Stop all other threads
    wii::OSThread::OSThread * p = wii::OSThread::currentThread->prev;
    while (p != nullptr)
    {
        wii::OSThread::OSSuspendThread(p);
        p = p->prev;
    }
    p = wii::OSThread::currentThread->next;
    while (p != nullptr)
    {
        wii::OSThread::OSSuspendThread(p);
        p = p->next;
    }

    // Print to OSReport
    wii::OSError::OSReport("%s\n", msg);

    const f32 topY = 50.0f;
    f32 bottomY = getBottomY(msg);
    f32 yShift = topY;
    f32 delta = bottomY <= SCREEN_BOTTOM ? 1.0f : 0.0f;
    f32 scale = isDolphin ? 0.7f : 0.55f; // dolphin uses a custom font for copyright reasons
    while (true)
    {
        // Check if power button was pressed
        if (spm::spmario::doShutdown)
        {
            wii::VI::VISetBlack(1);
            wii::VI::VIFlush();
            wii::VI::VIWaitForRetrace();
            wii::VI::VIWaitForRetrace();
            wii::VI::VIWaitForRetrace();
            wii::OSReset::OSShutdownSystem();
            while (true) {};
        }

        // Start frame
        wii::Mtx44 mtx;
        wii::DEMOInit::DEMOBeforeRender();
        wii::mtx::C_MTXOrtho(&mtx, 228.0f, -228.0f, -304.0f, 304.0f, 1.0f, 1000.0f);
        wii::GX::GXSetProjection(&mtx, 1);

        // Render text
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
    wii::OSError::OSReport(msg);

    // Store message to be drawn to screen
    size_t len = wii::string::strlen(msg);
    if ((head + len) >= sizeof(exceptionText))
        return;    
    wii::string::strcpy(exceptionText + head, msg);
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
        wii::OSError::OSReport("WARNING: Exception handler has crashed!\n");
        while (true) {};
    }

    if (inOSPanic)
        exceptionOSReport("WARNING: OSPanic handler has crashed!\n");
}

void exceptionPatch()
{   
    // OSPanic
    writeBranch(wii::OSError::OSPanic, 0x130, OSPanicForwarder); 

    // __OSUnhandledException
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x50,  exceptionOSReportForwarder);
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x1b0, exceptionOSReportForwarder);
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x1bc, exceptionOSReportForwarder);
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x1d8, exceptionOSReportForwarder);    
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x1ec, exceptionOSReportForwarder);
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x220, exceptionOSReportForwarder);
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x234, exceptionOSReportForwarder);
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x24c, exceptionOSReportForwarder);
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x264, exceptionOSReportForwarder);
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x274, exceptionOSReportForwarder);
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x28c, exceptionOSReportForwarder);
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x2a0, exceptionOSReportForwarder);
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x2b4, exceptionOSReportForwarder);
    writeBranchLink(wii::OSError::__OSUnhandledException, 0x2d0, exceptionOSReportForwarder);
    writeBranch(wii::OSError::__OSUnhandledException, 0x2d4, exceptionDraw);
    __OSUnhandledExceptionReal = patch::hookFunction(wii::OSError::__OSUnhandledException,
        [](int p1, int p2, int p3, int p4)
        {
            checkExceptionFlags();
            inException = true;
            __OSUnhandledExceptionReal(p1, p2, p3, p4);
        }
    );
    wii::OSError::exceptionMsg1[73] = '\n';
    wii::OSError::exceptionMsg2[75] = '\n';
    wii::OSError::exceptionMsg3[72] = '\n';

    // OSDumpContext
    writeBranchLink(wii::OSContext::OSDumpContext, 0x2c, exceptionOSReportForwarder);
    writeBranchLink(wii::OSContext::OSDumpContext, 0x58, exceptionOSReportForwarder);
    writeBranchLink(wii::OSContext::OSDumpContext, 0x7c, exceptionOSReportForwarder);
    writeBranchLink(wii::OSContext::OSDumpContext, 0x90, exceptionOSReportForwarder);
    writeBranchLink(wii::OSContext::OSDumpContext, 0x9c, exceptionOSReportForwarder);
    writeBranchLink(wii::OSContext::OSDumpContext, 0xc0, exceptionOSReportForwarder);
    writeBranchLink(wii::OSContext::OSDumpContext, 0x120, exceptionOSReportForwarder);
    writeBranchLink(wii::OSContext::OSDumpContext, 0x158, exceptionOSReportForwarder);
    writeBranchLink(wii::OSContext::OSDumpContext, 0x174, exceptionOSReportForwarder);
    writeBranchLink(wii::OSContext::OSDumpContext, 0x1ac, exceptionOSReportForwarder);
    writeBranchLink(wii::OSContext::OSDumpContext, 0x1fc, exceptionOSReportForwarder);
    writeBranchLink(wii::OSContext::OSDumpContext, 0x220, exceptionOSReportForwarder);
}
#else
void exceptionPatch()
{

}
#endif

}
