#include <common.h>
#include <spm/seqdrv.h>

#include "mod_ui_base/centredbutton.h"
#include "quitmenu.h"

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

            MenuWindow::sCurMenu->fullClose();
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
