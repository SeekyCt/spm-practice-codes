#include "mod_ui_base/window.h"
#include "patch.h"

#include <types.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/fontmgr.h>
#include <spm/homebuttondrv.h>
#include <spm/msgdrv.h>
#include <spm/windowdrv.h>
#include <wii/gx.h>
#include <wii/math.h>
#include <wii/mtx.h>
#include <ogc/gx.h>

namespace mod {

Window * Window::sWindowList = nullptr;
static const wii::RGBA white {0xff, 0xff, 0xff, 0xff};

Window::Window()
{
    // Add to linked list
    mNext = sWindowList;
    sWindowList = this;

    // Display on default camera
    mCamera = spm::camdrv::CAM_DEBUG_3D;
}

Window::~Window()
{
    // Remove from linked list
    if (sWindowList == this)
    {
        // If this is the first window in the list, set the next window as the first
        sWindowList = mNext;
    }
    else
    {
        // If this isn't the first window, search for the window before it
        Window * prev = sWindowList;
        while (prev->mNext != this)
            prev = prev->mNext;
        
        // Set the previous window's next to this window's next
        prev->mNext = mNext;
    }
}

void Window::disp()
{
    // stub for inheriting classes to replace
}

void Window::drawString(const char * str, f32 x, f32 y, const wii::RGBA * colour,
                        f32 scale, bool edge, bool noise, bool rainbow)
{
    // Handle colour so alpha is ready for draw start
    wii::RGBA _colour;
    if (colour != nullptr)
        _colour = *colour;
    else
        _colour = {0xff, 0xff, 0xff, 0xff};

    // Initialise
	spm::fontmgr::FontDrawStart_alpha(_colour.a);

    // Handle settings
	spm::fontmgr::FontDrawScale(scale);

    _colour.a = 0xff; // Alpha doesn't work here, so override it to max
    spm::fontmgr::FontDrawColor(&_colour);
    
    if (edge)
        spm::fontmgr::FontDrawEdge();
    else
    	spm::fontmgr::FontDrawEdgeOff();
	
    if (noise)
    	spm::fontmgr::FontDrawNoise();
    else
    	spm::fontmgr::FontDrawNoiseOff();
    
    if (rainbow)
        spm::fontmgr::FontDrawRainbowColor();
    else
        spm::fontmgr::FontDrawRainbowColorOff();

    // Draw
    spm::fontmgr::FontDrawString(x, y, str);	
}

void Window::drawMessage(const char * str, s32 x, s32 y, const wii::RGBA * colour,
                        f32 scale, bool edge, bool noise, bool rainbow)
{
    // Handle colour so alpha is ready for draw start
    wii::RGBA _colour;
    if (colour != nullptr)
        _colour = *colour;
    else
        _colour = {0xff, 0xff, 0xff, 0xff};

    // Initialise
	spm::fontmgr::FontDrawStart_alpha(_colour.a);

    // Handle settings
	spm::fontmgr::FontDrawScale(scale);

    _colour.a = 0xff; // Alpha doesn't work here, so override it to max
    spm::fontmgr::FontDrawColor(&_colour);
    
    if (edge)
        spm::fontmgr::FontDrawEdge();
    else
    	spm::fontmgr::FontDrawEdgeOff();
	
    if (noise)
    	spm::fontmgr::FontDrawNoise();
    else
    	spm::fontmgr::FontDrawNoiseOff();
    
    if (rainbow)
        spm::fontmgr::FontDrawRainbowColor();
    else
        spm::fontmgr::FontDrawRainbowColorOff();

    // Convert message name to string and draw
    spm::fontmgr::FontDrawMessage(x, y, str);	
}

void Window::drawMessageSearch(const char * name, s32 x, s32 y, const wii::RGBA * colour,
                               f32 scale, bool edge, bool noise, bool rainbow)
{
    drawMessage(spm::msgdrv::msgSearch(name), x, y, colour, scale, edge, noise, rainbow);    
}

void Window::drawBox(u16 GXTexMapID, const wii::RGBA * colour, f32 x, f32 y, f32 width,
             f32 height, f32 curve)
{
    // windowDispGX_Waku_col doesn't exist in this game, so an identity matrix
    // is used in GX2 to create an equivalent
    wii::Mtx34 mtx;
    wii::mtx::PSMTXIdentity(&mtx);
    spm::windowdrv::windowDispGX2_Waku_col(&mtx, GXTexMapID, colour, x, y, width, height,
                                           curve);
}

void Window::setupGX(s32 zMode)
{
    // Disable some stuff
    wii::GX::GXSetCullMode(GX_CULL_NONE); // disable all culling
    wii::GX::GXSetZCompLoc(true); // compare z before texture
    wii::GX::GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0); // set alpha test to always pass
    wii::GX::GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR); // write output directly to EFB
    wii::GX::GXSetZMode(true, zMode, true); // always pass z test
    wii::GX::GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, &white); // disable fog

    // Set verex input format
    wii::GX::GXClearVtxDesc(); // reset vertex properties
    wii::GX::GXSetVtxDesc(GX_VA_POS, GX_DIRECT); // position coordinates first
    wii::GX::GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT); // colour second
    wii::GX::GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); // position format is a f32 vec3
    wii::GX::GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0); // color format is rgba8

    // Setup for direct colour
    wii::GX::GXSetNumChans(1); // enable 1 colour channel
    wii::GX::GXSetNumTexGens(0); // disable tex coord generation
    wii::GX::GXSetNumTevStages(1); // only 1 TEV stage
    wii::GX::GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0); // disable textures in TEV
        // set colour to only come from the vertices
    wii::GX::GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE); 
    wii::GX::GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR); // set TEV to pass the colour directly

    // Setup view matrix
    spm::camdrv::CamEntry * camera = spm::camdrv::camGetCurPtr();
    wii::GX::GXLoadPosMtxImm(&camera->viewMtx, 0);
    wii::GX::GXSetCurrentMtx(0);
}

void Window::drawBoxGX(const wii::RGBA * colour, f32 x, f32 y, f32 width, f32 height)
{
    setupGX(GX_ALWAYS);

    // Draw
    f32 top = y;
    f32 bottom = y - height;
    f32 left = x;
    f32 right = x + width;
    f32 z = 0.0f;
    u32 _colour = *reinterpret_cast<const u32 *>(colour);
    wii::GX::GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    {
        ogc::GX::GX_Position3f32(left, top, z);
        ogc::GX::GX_Color1u32(_colour);
        ogc::GX::GX_Position3f32(right, top, z);
        ogc::GX::GX_Color1u32(_colour);
        ogc::GX::GX_Position3f32(right, bottom, z);
        ogc::GX::GX_Color1u32(_colour);
        ogc::GX::GX_Position3f32(left, bottom, z);
        ogc::GX::GX_Color1u32(_colour);
    }
    GXEnd();
}

void Window::drawLineCuboidGX(const wii::RGBA * colour, f32 top, f32 bottom, f32 left, f32 right, f32 front, f32 back)
{
    setupGX(GX_LEQUAL);

    // Get colour
    u32 _colour = *reinterpret_cast<const u32 *>(colour);

    // Draw front face & top left line
    wii::GX::GXBegin(GX_LINESTRIP, GX_VTXFMT0, 6);
    {
        /*
              6
             /
            1--2
            |  |
            4--3
        */

        // 1
        ogc::GX::GX_Position3f32(left, top, front);
        ogc::GX::GX_Color1u32(_colour);

        // 2
        ogc::GX::GX_Position3f32(right, top, front);
        ogc::GX::GX_Color1u32(_colour);

        // 3
        ogc::GX::GX_Position3f32(right, bottom, front);
        ogc::GX::GX_Color1u32(_colour);

        // 4
        ogc::GX::GX_Position3f32(left, bottom, front);
        ogc::GX::GX_Color1u32(_colour);

        // 5 (1)
        ogc::GX::GX_Position3f32(left, top, front);
        ogc::GX::GX_Color1u32(_colour);

        // 6
        ogc::GX::GX_Position3f32(left, top, back);
        ogc::GX::GX_Color1u32(_colour);
    }
    GXEnd();


    // Draw back face & top right line
    wii::GX::GXBegin(GX_LINESTRIP, GX_VTXFMT0, 6);
    {
        /*
            2--1
            |  | \
            3--4  6
        */

        // 1
        ogc::GX::GX_Position3f32(right, top, back);
        ogc::GX::GX_Color1u32(_colour);

        // 2
        ogc::GX::GX_Position3f32(left, top, back);
        ogc::GX::GX_Color1u32(_colour);

        // 3
        ogc::GX::GX_Position3f32(left, bottom, back);
        ogc::GX::GX_Color1u32(_colour);

        // 4
        ogc::GX::GX_Position3f32(right, bottom, back);
        ogc::GX::GX_Color1u32(_colour);

        // 5 (1)
        ogc::GX::GX_Position3f32(right, top, back);
        ogc::GX::GX_Color1u32(_colour);

        // 6
        ogc::GX::GX_Position3f32(right, top, front);
        ogc::GX::GX_Color1u32(_colour);
    }
    GXEnd();

    // Draw bottom lines
    wii::GX::GXBegin(GX_LINES, GX_VTXFMT0, 4);
    {
        /*
              2   4
             /   /
            1   3
        */

        // 1
        ogc::GX::GX_Position3f32(left, bottom, front);
        ogc::GX::GX_Color1u32(_colour);

        // 2
        ogc::GX::GX_Position3f32(left, bottom, back);
        ogc::GX::GX_Color1u32(_colour);

        // 3
        ogc::GX::GX_Position3f32(right, bottom, front);
        ogc::GX::GX_Color1u32(_colour);

        // 4
        ogc::GX::GX_Position3f32(right, bottom, back);
        ogc::GX::GX_Color1u32(_colour);
    }
    GXEnd();
}

void Window::drawLineCubeGX(const wii::RGBA * colour, f32 x, f32 y, f32 z, f32 sideLength)
{
    f32 len = sideLength / 2.0f;
    f32 left = x - len;
    f32 right = x + len;
    f32 top = y + len;
    f32 bottom = y - len;
    f32 front = z + len;
    f32 back = z - len;
    drawLineCuboidGX(colour, top, bottom, left, right, front, back);
}


void Window::circleGX(const wii::RGBA * colour, f32 x, f32 y, f32 z, f32 radius, s32 n)
{
    f32 angleStep = PIx2 / (float) n;

    // Get colour
    u32 _colour = *reinterpret_cast<const u32 *>(colour);

    wii::GX::GXBegin(GX_LINESTRIP, GX_VTXFMT0, n + 1);
    for (f32 a = 0.0f; a < PIx2; a += angleStep)
    {
        f32 _x = wii::math::cos(a) * radius;
        f32 _z = wii::math::sin(a) * radius;
        ogc::GX::GX_Position3f32(x + _x, y, z + _z);
        ogc::GX::GX_Color1u32(_colour);
    }
    ogc::GX::GX_Position3f32(x + radius, y, z);
    ogc::GX::GX_Color1u32(_colour);
    GXEnd();
}

void Window::cylinderBarsGX(const wii::RGBA * colour, f32 x, f32 y, f32 z, f32 radius, f32 height, s32 n)
{
    f32 angleStep = PIx2 / (float) n;

    // Get colour
    u32 _colour = *reinterpret_cast<const u32 *>(colour);

    wii::GX::GXBegin(GX_LINES, GX_VTXFMT0, n * 2);
    for (f32 a = 0.0f; a < PIx2; a += angleStep)
    {
        f32 _x = wii::math::cos(a) * radius;
        f32 _z = wii::math::sin(a) * radius;
        ogc::GX::GX_Position3f32(x + _x, y, z + _z);
        ogc::GX::GX_Color1u32(_colour);
        ogc::GX::GX_Position3f32(x + _x, y + height, z + _z);
        ogc::GX::GX_Color1u32(_colour);
    }
    GXEnd();
}

void Window::drawLineCylinderGX(const wii::RGBA * colour, f32 x, f32 y, f32 z, f32 radius, f32 height, s32 n, s32 barN)
{
    setupGX(GX_LEQUAL);
    circleGX(colour, x, y, z, radius, n);
    cylinderBarsGX(colour, x, y, z, radius, height, barN);
    circleGX(colour, x, y + height, z, radius, n);
}

void Window::windowDisp(s8 camId, void * param)
{
    // Disp callback param isn't needed
    (void) param;

    // Call every window's display function
    Window * w = sWindowList;
    while (w != nullptr)
    {
        // Window may delete itself during disp so this has to be read now
        Window * temp = w->mNext;

        // Call custom display function if window is on this camera
        if (w->mCamera == camId)
            w->disp();

        // Continue to next window
        w = temp;
    }
}

void Window::windowMain()
{
    if ((spm::homebuttondrv::homebuttonWp->flags & HOMEBUTTON_FLAG_OPEN) == 0)
    {
        // Schedule windowDisp to run this frame on all cameras
        for (s32 i = 0; i < spm::camdrv::CAM_ID_MAX; i++)
            spm::dispdrv::dispEntry(i, 2, 0.0f, Window::windowDisp, nullptr);
    }
}

void Window::homebuttonDispPatch()
{
    // Change homebutton to display on debug 3d camera to display on top of mod graphics
#if (defined SPM_EU0 || defined SPM_EU1)
    #define HOMEBUTTON_MAIN_CAM_OFFSET 0x844
#elif (defined SPM_US0 || defined SPM_JP0)
    #define HOMEBUTTON_MAIN_CAM_OFFSET 0x804
#elif (defined SPM_US1 || defined SPM_US2 || defined SPM_JP1)
    #define HOMEBUTTON_MAIN_CAM_OFFSET 0x814
#elif (defined SPM_KR0)
    #define HOMEBUTTON_MAIN_CAM_OFFSET 0x714
#endif
    writeWord(spm::homebuttondrv::homebuttonMain, HOMEBUTTON_MAIN_CAM_OFFSET, 0x3860000e); // li r3, CAM_DEBUG_3D
}

}
