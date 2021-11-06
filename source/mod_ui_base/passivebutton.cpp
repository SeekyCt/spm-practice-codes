#include "nandsettings.h"
#include "mod_ui_base/passivebutton.h"

#include <spm/wpadmgr.h>
#include <wii/wpad.h>

namespace mod {

PassiveButton::PassiveButton(class MenuWindow * parent, const char * msg, f32 x, f32 y,
                             u32 passiveBtnMask, MenuAction * action, void * actionParam,
                             f32 scale, wii::RGBA * colour, bool drawAsMessage)
                            :MenuButton(parent, msg, x, y, action, actionParam, false,
                                        scale, colour, nullptr, drawAsMessage)
{
    mPassiveBtnMask = passiveBtnMask;
}

void PassiveButton::handlePassiveInputs(u32 btn, u32 btnRpt)
{
    (void) btnRpt;
    
    // Handle action
    if (mAction != nullptr && (btn & mPassiveBtnMask))
    {
        bool ret = mAction(this, mActionParam);

        // If this button has been deleted, stop
        if (!ret)
            return;
    }
}

}
