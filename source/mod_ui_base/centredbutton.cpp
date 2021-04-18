#include "mod_ui_base/centredbutton.h"

#include <types.h>
#include <spm/fontmgr.h>

namespace mod {

void CentredButton::disp(f32 centreX, f32 centreY, bool selected)
{
    wii::RGBA * colour = selected ? &mSelColour : &mColour;
    f32 x = centreX - ((spm::fontmgr::FontGetMessageWidth(mMsg) * mScale) / 2);
    mWindow->drawString(mMsg, x, mPosY + centreY, colour, mScale, true);
}

CentredButton::CentredButton(MenuWindow * parent, const char * msg, f32 y,
                             MenuAction * action, void * actionParam, bool autoPress,
                             f32 scale, wii::RGBA * colour, wii::RGBA * selColour)
    : MenuButton(parent, msg, 0, y, action, actionParam, autoPress, scale, colour, selColour)
{
    
}

}
