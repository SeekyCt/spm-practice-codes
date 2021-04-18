#include "mainmenu.h"
#include "nandmenu.h"
#include "nandsettings.h"

#include <types.h>
#include <spm/evtmgr.h>
#include <spm/fontmgr.h>
#include <wii/string.h>

namespace mod {

bool NandMenu::loadSettings(MenuButton * button, void * param)
{
    (void) param;

    NandMenu * instance = reinterpret_cast<NandMenu *>(param);

    instance->mTask = NandTask::LOAD;
    instance->mEvtId = spm::evtmgr::evtEntryType(nand_settings_load, 0, 0, 0)->id;
    instance->mMiddleMsg = "Loading settings...";
    instance->removeButtons();

    return true;
}

bool NandMenu::saveSettings(MenuButton * button, void * param)
{
    (void) button;
    
    NandMenu * instance = reinterpret_cast<NandMenu *>(param);

    instance->mTask = NandTask::SAVE;
    instance->mEvtId = spm::evtmgr::evtEntryType(nand_settings_write, 0, 0, 0)->id;
    instance->mMiddleMsg = "Saving settings...";
    instance->removeButtons();

    return true;
}

bool NandMenu::defaultSettings(MenuButton * button, void * param)
{
    (void) button;

    NandMenu * instance = reinterpret_cast<NandMenu *>(param);

    nandSettingsDefaults();
    instance->mMiddleMsg = "Loaded default settings!";

    return true;
}

bool NandMenu::deleteSettings(MenuButton * button, void * param)
{
    (void) param;

    NandMenu * instance = reinterpret_cast<NandMenu *>(param);

    instance->mTask = NandTask::DELETE;
    instance->mEvtId = spm::evtmgr::evtEntryType(nand_settings_delete, 0, 0, 0)->id;
    instance->mMiddleMsg = "Deleting saved settings...";
    instance->removeButtons();

    return true;
}

void NandMenu::disp()
{
    // Run main display function
    MenuWindow::disp();

    // Menu may have been closed during that function
    if (mShouldClose)
    {
        // Change back to parent menu
        delete MenuWindow::sCurMenu;
        MenuWindow::sCurMenu = new MainMenu();
    }
    else
    {
        // Draw status message in the centre if one exists
        if (mMiddleMsg != nullptr)
            drawStringCentre(mMiddleMsg, 0);    

        // If a process is running, check if it's done
        if ((mTask != NandTask::NONE) && !spm::evtmgr::evtCheckId(mEvtId))
        {
            // Inform user of completion
            switch (mTask)
            {
                case NandTask::LOAD:
                    if (nandSettingsSuccess)
                        mMiddleMsg = "Loaded settings!";
                    else
                        mMiddleMsg = "No settings to load.";
                    break;
                case NandTask::SAVE:
                    if (nandSettingsSuccess)
                        mMiddleMsg = "Saved settings!";
                    else
                        mMiddleMsg = "Failed to save settings.";
                    break;
                case NandTask::DELETE:
                    if (nandSettingsSuccess)
                        mMiddleMsg = "Deleted saved settings!";
                    else
                        mMiddleMsg = "No saved settings to delete.";
                    break;
                default:
                    break;
            }

            // Signal finishing
            mTask = NandTask::NONE;
            
            // Bring back buttons
            spawnButtons();
        }
    }
}

void NandMenu::spawnButtons()
{
    // Create buttons
    f32 y = 180;
    mButtonPtrs[0] = new MenuButton(this, "Load Settings from NAND", -330.0f, y, loadSettings, this);
    y -= FONT_HEIGHT + 5;
    mButtonPtrs[1] = new MenuButton(this, "Save Settings to NAND", -330.0f, y, saveSettings, this);
    y -= FONT_HEIGHT + 5;
    mButtonPtrs[2] = new MenuButton(this, "Restore Default Settings", -330.0f, y, defaultSettings, this);
    y -= FONT_HEIGHT + 5;
    mButtonPtrs[3] = new MenuButton(this, "Delete Settings on NAND", -330.0f, y, deleteSettings, this);

    for (int i = 0; i < 3; i++)
        buttonLinkVertical(mButtonPtrs[i], mButtonPtrs[i + 1]);

    // Restore selected button
    mCurButton = mButtonPtrs[mCurButtonIdx];
}

void NandMenu::removeButtons()
{
    // Back up selected button
    for (int i = 0; i < 4; i++)
    {
        if (mCurButton == mButtonPtrs[i])
        {
            mCurButtonIdx = i;
            break;
        }
    }

    // Delete buttons
    mButtons = nullptr;
    for (int i = 0; i < 4; i++)
    {
        delete mButtonPtrs[i];
        mButtonPtrs[i] = nullptr;
    }

    // Deselect
    mCurButton = nullptr;
}

NandMenu::NandMenu()
{
    mTask = NandTask::NONE;
    mMiddleMsg = nullptr;
    mShouldClose = false;
    mCurButtonIdx = 0;
    spawnButtons();
    mTitle = "SPM Practice Codes v1.0";
}

void NandMenu::close()
{
    // Don't close if busy (technically not needed since mCurButton is null but that may change)
    if (mTask != NandTask::NONE)
        return;

    // Schedule changing back to parent menu
    // (can't just delete now since this will return to the rest of the custom disp function)
    mShouldClose = true;
}

}
