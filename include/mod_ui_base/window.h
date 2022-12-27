/*
    Window
    Abstract base window class
*/

#pragma once

#include <common.h>
#include <wii/tpl.h>
#include <wii/gx.h>

namespace mod {

#define FONT_HEIGHT 20
#define FONT_WIDTH 20
#define FONT_ARROW_HEIGHT 26
#define FONT_NUM_LEFT_GAP 1
#define FONT_NUM_WIDTH 14
#define FONT_NUM_TOP_GAP 5

class Window
{
private:
    Window * mNext;

    static Window * sWindowList;
    static void windowDisp(s32 camId, void * param);
    static void setupGX(s32 zMode);
    static void circleGX(const wii::gx::GXColor * colour, f32 x, f32 y, f32 z,
                         f32 radius, s32 n);
    static void cylinderBarsGX(const wii::gx::GXColor * colour, f32 x, f32 y, f32 z,
                               f32 radius, f32 height, s32 n);

protected:
    s32 mCamera;

    virtual void preDisp();
    virtual void disp();

public:
    /*
        Abstract, don't do anything but are used by most classes inheriting
        Windows are drawn on the debug3d layer so generally:
            x: -375.0 to 375.0 left to right
            y: -225.0 to 225.0 bottom to top
    */
    f32 mPosX, mPosY;
    bool mDrawOverHbm;

    Window();
    virtual ~Window();

    static void drawString(const char * str,
                           f32 x, f32 y,
                           const wii::gx::GXColor * colour = nullptr,
                           f32 scale = 1.0f,
                           bool edge = false,
                           bool noise = false,
                           bool rainbow = false);
    static void drawMessage(const char * str,
                            s32 x, s32 y,
                            const wii::gx::GXColor * colour = nullptr,
                            f32 scale = 1.0f,
                            bool edge = false,
                            bool noise = false,
                            bool rainbow = false);
    static void drawMessageSearch(const char * name,
                                  s32 x, s32 y,
                                  const wii::gx::GXColor * colour = nullptr,
                                  f32 scale = 1.0f,
                                  bool edge = false,
                                  bool noise = false,
                                  bool rainbow = false);
    static void drawBox(u16 GXTexMapID, const wii::gx::GXColor * colour,
                        f32 x, f32 y, f32 width, f32 height, f32 curve);
    static void drawBoxGX(const wii::gx::GXColor * colour, f32 x, f32 y,
                          f32 width, f32 height);
    static void drawLineCuboidGX(const wii::gx::GXColor * colour, f32 top, f32 bottom,
                                 f32 left, f32 right, f32 front, f32 back);
    static void drawLineCubeGX(const wii::gx::GXColor * colour, f32 x, f32 y, f32 z,
                               f32 sideLength);
    void drawLineCylinderGX(const wii::gx::GXColor * colour, f32 x, f32 y, f32 z,
                            f32 radius, f32 height, s32 n, s32 barN);
    static void drawTexture(wii::tpl::TPLHeader * tpl, u32 texId, f32 x, f32 y,
                            f32 scale, const wii::gx::GXColor * colour);

    static void windowMain();
};

}
