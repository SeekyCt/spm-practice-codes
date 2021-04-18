#include "mod_ui_base/menuicon.h"

#include <types.h>
#include <spm/icondrv.h>
#include <spm/windowdrv.h>
#include <wii/mtx.h>
#include <wii/OSError.h>

namespace mod {


MenuIcon::MenuIcon(class MenuWindow * parent, int iconId, f32 x, f32 y, MenuAction * action, void * actionParam,
                   bool autoPress, f32 scale, wii::RGBA * selColour)
                  :MenuButton(parent, nullptr, x, y, action, actionParam, autoPress, scale, nullptr, selColour)
{
    // Store extra parameter
    mIconId = iconId;
}

void MenuIcon::disp(f32 centreX, f32 centreY, bool selected)
{
    const f32 x = centreX + mPosX;
    const f32 y = centreY + mPosY;

    // TODO: handle scale

    // Draw background if highlighted
    if (selected)
    {
        // Draw background - windowDispGX_Waku_col doesn't exist in this game,
        // so an identity matrix is in GX2 to create an equivalent
        wii::Mtx34 mtx;
        wii::mtx::PSMTXIdentity(&mtx);
        spm::windowdrv::windowDispGX2_Waku_col(&mtx, 0, &mSelColour, x, y, 50, 50, 0);
    }

    // Draw icon
    wii::Vec3 pos {x + 25.0f, y - 50.0f, 0.0f};
    spm::icondrv::iconDispGx(mScale, &pos, 0x18, mIconId);
}

}
