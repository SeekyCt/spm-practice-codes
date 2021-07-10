#include "mod_ui_base/window.h"

#include <types.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/fontmgr.h>
#include <spm/homebutton.h>
#include <spm/msgdrv.h>
#include <spm/windowdrv.h>
#include <wii/mtx.h>

namespace mod {

Window * Window::sWindowList = nullptr;

Window::Window()
{
    // Add to linked list
    mNext = sWindowList;
    sWindowList = this;
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

void Window::windowDisp(s8 camId, void * param)
{
    // Disp callback params aren't needed
    (void) camId;
    (void) param;

    // Call every window's display function
    Window * w = sWindowList;
    while (w != nullptr)
    {
        // Window may delete itself during disp so this has to be read now
        Window * temp = w->mNext;

        // Call custom display function
        w->disp();

        // Continue to next window
        w = temp;
    }
}

void Window::windowMain()
{
    if ((spm::homebutton::homebuttonWp->flags & HOMEBUTTON_FLAG_OPEN) == 0)
    {
        // Schedule windowDisp to run this frame on the debug 3d camera
        spm::dispdrv::dispEntry(spm::camdrv::CAM_DEBUG_3D, 2, 0.0f, Window::windowDisp, 0);
    }
}

}
