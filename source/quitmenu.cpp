#include "mod_ui_base/centredbutton.h"
#include "quitmenu.h"

#include <types.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>

namespace mod {

QuitMenu::QuitMenu()
{
    // Create buttons
    CentredButton * top = new CentredButton(this, "Yes", 40.0f, 
        [](MenuButton * button, void * param)
        {
            (void) button;
            (void) param;

            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "title", "");

            delete MenuWindow::sCurMenu;
            MenuWindow::sCurMenu = nullptr;
            return false;
        }
    );
    CentredButton * bottom = new CentredButton(this, "No", 0.0f,
        [](MenuButton * button, void * param)
        {
            (void) button;

            QuitMenu * instance = reinterpret_cast<QuitMenu *>(param);
            instance->close();

            return false;
        }, 
        this
    );
    buttonLinkVertical(top, bottom);
    
    // Set title and selected button
    mCurButton = bottom;
    mTitle = "Are you sure you want to return to Title Screen?";
}

}
