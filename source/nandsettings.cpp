#include "evt_cmd.h"
#include "evtnandapi.h"
#include "nandsettings.h"
#include "patch.h"
#include "util.h"

#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/relmgr.h>
#include <wii/NAND.h>
#include <wii/OSError.h>
#include <wii/stdio.h>
#include <wii/string.h>

namespace mod {

// Data to be passed into NAND lib
static u8 _settings[ROUND_UP_32(sizeof(NandSettings))] __attribute__((aligned(32)));
static u8 openBuf[0x4000] __attribute__((aligned(32)));
static wii::NAND::NANDFileInfo fileInfo;
static wii::NAND::NANDCommandBlock commandBlock;

// Actual settings struct
NandSettings * gSettings = reinterpret_cast<NandSettings *>(&_settings);

// Feedback for script callers
int gNandSettingsSuccess;

// Error handling helpers
static wii::RGBA errorFg {0xff, 0xff, 0xff, 0xff};
static wii::RGBA errorBg {0x00, 0x00, 0x00, 0xff};
static char errorMsg[256];
#define ERROR(ret) \
    do { \
    wii::stdio::sprintf(errorMsg, "SPM Practice Codes Unhandled NAND error %d\nin %s\n(please report this)", (ret), __FUNCTION__); \
    wii::OSError::OSFatal(&errorFg, &errorBg, errorMsg); \
    } while (false)

// evt_nandsettings_handle_read_output()
EVT_DECLARE_USER_FUNC(evt_nandsettings_handle_read_output, 0)
// evt_nandSettingsWrite(&ret)
EVT_DECLARE_USER_FUNC(evt_nandSettingsWrite, 1)
// evt_nandSettingsOpen(mode, &ret)
EVT_DECLARE_USER_FUNC(evt_nandSettingsOpen, 2)
// evt_nandSettingsClose(&ret)
EVT_DECLARE_USER_FUNC(evt_nandSettingsClose, 1)
// evt_nandSettingsCheck(&ret)
EVT_DECLARE_USER_FUNC(evt_nandSettingsCheck, 1)
// evt_nandSettingsFail(ret)
EVT_DECLARE_USER_FUNC(evt_nandSettingsFail, 1)

EVT_BEGIN(nand_settings_load)
    // Try open settings file
    USER_FUNC(evt_nandSettingsOpen, NAND_MODE_READ, LW(0))

    IF_EQUAL(LW(0), NAND_CODE_OK)
        // If file opened, read it
        USER_FUNC(evt_nand_read, PTR(&fileInfo), PTR(_settings), sizeof(_settings), PTR(&commandBlock), LW(0))
        USER_FUNC(evt_nandsettings_handle_read_output)
        IF_LARGE_EQUAL(LW(0), 0)
            // Clean up if successful
            USER_FUNC(evt_nandSettingsClose, LW(0))
            IF_EQUAL(LW(0), 0)
                // If it closed, signal loading
                SET_RAM(1, PTR(&gNandSettingsSuccess))
                RETURN()
            END_IF()
        END_IF()
    ELSE()
        IF_EQUAL(LW(0), NAND_CODE_NOT_EXIST)
            // Signal not loading
            SET_RAM(0, PTR(&gNandSettingsSuccess))
            RETURN()
        END_IF()
    END_IF()

    // If an unhandled NAND error happened, kill the game
    USER_FUNC(evt_nandSettingsFail, LW(0))
EVT_END()

#define retry 1
EVT_BEGIN(nand_settings_write)
    LBL(retry)

    // Try open settings file
    USER_FUNC(evt_nandSettingsOpen, NAND_MODE_WRITE, LW(0))

    // Create file if needed
    IF_EQUAL(LW(0), NAND_CODE_NOT_EXIST)
        // Check creating it is possible
        USER_FUNC(evt_nandSettingsCheck, LW(0))

        // Try to create if that succeeded
        IF_EQUAL(LW(0), 0)
            USER_FUNC(evt_nand_create, PTR(SETTINGS_FILE_NAME), NAND_PERMISSION_READ_WRITE,
                      0, PTR(&commandBlock), LW(0))
        END_IF()

        IF_EQUAL(LW(0), 0)
            // Retry if both succeeded
            GOTO(retry)
        ELSE()
            // If either failed, exit early
            SET_RAM(0, PTR(&gNandSettingsSuccess))
            RETURN()
        END_IF()
    END_IF()

    IF_EQUAL(LW(0), NAND_CODE_OK)
        // If file opened, write it
        USER_FUNC(evt_nandSettingsWrite, LW(0))

        IF_EQUAL(LW(0), (s32)sizeof(_settings))
            // Clean up if successful
            USER_FUNC(evt_nandSettingsClose, LW(0))
            IF_EQUAL(LW(0), 0)
                SET_RAM(1, PTR(&gNandSettingsSuccess))
                RETURN()
            END_IF()
        END_IF()
    END_IF()

    // If an unhandled NAND error happened, kill the game
    USER_FUNC(evt_nandSettingsFail, LW(0))
EVT_END()
#undef retry

EVT_BEGIN(nand_settings_delete)
    // Try open settings file to check it exists
    USER_FUNC(evt_nandSettingsOpen, NAND_MODE_WRITE, LW(0))

    IF_EQUAL(LW(0), NAND_CODE_OK)
        // Try close file
        USER_FUNC(evt_nandSettingsClose, LW(0))

        IF_EQUAL(LW(0), NAND_CODE_OK)
            // Try deleting settings file
            USER_FUNC(evt_nand_delete, PTR(SETTINGS_FILE_NAME), PTR(&commandBlock), LW(0))

            IF_EQUAL(LW(0), NAND_CODE_OK)
                // Signal success
                SET_RAM(1, PTR(&gNandSettingsSuccess))
                RETURN()
            END_IF()
        END_IF()
    ELSE()
        IF_EQUAL(LW(0), NAND_CODE_NOT_EXIST)
            // Signal there's no file to delete
            SET_RAM(0, PTR(&gNandSettingsSuccess))
            RETURN()
        END_IF()
    END_IF()

    // If an unhandled NAND error happened, kill the game
    USER_FUNC(evt_nandSettingsFail, LW(0))
EVT_END()

struct AsyncResult
{
    bool set;
    s32 val;
};
static AsyncResult asyncResult;

static void cb(s32 result, wii::NAND::NANDCommandBlock * cmd)
{
    (void) cmd;
    asyncResult.val = result;
    asyncResult.set = true;
}

s32 evt_nandsettings_handle_read_output(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    // Dump contents for debugging
    wii::OSError::OSReport("nandsettings: read at %x, contents:\n", (u32) &_settings);
    u32 * d = reinterpret_cast<u32 *>(gSettings);
    for (u32 i = 0; i < (sizeof(*gSettings) / sizeof(u32)); i += 4)
        wii::OSError::OSReport("%08x %08x %08x %08x\n", d[i], d[i+1], d[i+2], d[i+3]);

    // Handle settings version
    switch (gSettings->version)
    {
        case 1:
            NandSettingsV1 v1;
            wii::string::memcpy(&v1, gSettings, sizeof(v1));
            wii::OSError::OSReport("nandsettings: updating settings v1->2.\n");

            // Move relocated settings
            // hudMapDoor and hudXYZ are already in place
            gSettings->mapChangeEffect = v1.mapChangeEffect;

            // Initialise new settings
            gSettings->xyzInterval = 4;
            gSettings->xyzDP = 2;

            // Increment version
            gSettings->version = 2;

            // Fall through into v2->3

        case 2:
            wii::OSError::OSReport("nandsettings: updating settings v2->3.\n");

            // Existing settings are already in place

            // Initialise new setting
            gSettings->customPitText = false;

            // Increment version
            gSettings->version = 3;
            break;

        case SETTINGS_VER:
            wii::OSError::OSReport("nandsettings: settings version ok.\n");
            break;

        default:
            wii::OSError::OSReport("nandsettings: settings file too new, using defaults\n");
            nandSettingsDefaults();
            break;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_nandSettingsWrite(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    // On first run, try read the file
    if (firstRun)
    {
        asyncResult.set = false;
        s32 ret = wii::NAND::NANDWriteAsync(&fileInfo, &_settings, sizeof(_settings), cb, &commandBlock);
        if (ret < 0)
            ERROR(ret);
    }

    // If the async process has finished, return to script
    if (asyncResult.set)
    {
        wii::OSError::OSReport("nandsettings: wrote with result %d\n", asyncResult.val);
        spm::evtmgr_cmd::evtSetValue(entry, entry->pCurData[0], asyncResult.val);
        return 2;
    }
    else
    {
        return 0;
    }
}

s32 evt_nandSettingsOpen(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    // On first run, try open the file
    if (firstRun)
    {
        asyncResult.set = false;
        s32 ret = wii::NAND::NANDSafeOpenAsync(SETTINGS_FILE_NAME, &fileInfo, entry->pCurData[0], openBuf, sizeof(openBuf), cb, &commandBlock);
        if (ret < 0)
            ERROR(ret);
    }

    // If the async process has finished, return to script
    if (asyncResult.set)
    {
        wii::OSError::OSReport("nandsettings: opened with result %d\n", asyncResult.val);
        spm::evtmgr_cmd::evtSetValue(entry, entry->pCurData[1], asyncResult.val);
        return 2;
    }
    else
    {
        return 0;
    }
}

s32 evt_nandSettingsClose(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    // On first run, try close file
    if (firstRun)
    {
        asyncResult.set = false;
        s32 ret = wii::NAND::NANDSafeCloseAsync(&fileInfo, cb, &commandBlock);;
        if (ret < 0)
            ERROR(ret);
    }

    // If the async process has finished, return to script
    if (asyncResult.set)
    {
        wii::OSError::OSReport("nandsettings: closed with result %d\n", asyncResult.val);
        spm::evtmgr_cmd::evtSetValue(entry, entry->pCurData[0], asyncResult.val);
        return 2;
    }
    else
    {
        return 0;
    }
}

static u32 checkAnswer;
s32 evt_nandSettingsCheck(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    // On first run, start check process
    if (firstRun)
    {
        asyncResult.set = false;
        s32 ret = wii::NAND::NANDCheckAsync(
            1, 1, &checkAnswer,
            [](s32 result, wii::NAND::NANDCommandBlock * cmd)
            {
                // Store result
                asyncResult.val = result;
                
                // If there was no error checking, review the answer
                if (result == NAND_CODE_OK)
                {
                    (void) cmd;

                    // Game checks for these separately, but both mean we shouldn't continue
                    if (checkAnswer & (0xa | 0x5))
                        // Make an error i
                        asyncResult.val = -128;

                    asyncResult.set = true;
                }
            },
            &commandBlock
        );
        if (ret < 0)
            ERROR(ret);
    }

    // If the async process has finished, return to script
    if (asyncResult.set)
    {
        wii::OSError::OSReport("nandsettings: checked with result %d\n", asyncResult.val);
        spm::evtmgr_cmd::evtSetValue(entry, entry->pCurData[0], asyncResult.val);
        return 2;
    }
    else
    {
        return 0;
    }
}

s32 evt_nandSettingsFail(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    if (firstRun)
    {
        wii::stdio::sprintf(errorMsg, "SPM Practice Codes NAND error %d\nin callback\n(please report this)", 
                            spm::evtmgr_cmd::evtGetValue(entry, entry->pCurData[0]));
        wii::OSError::OSFatal(&errorFg, &errorBg, errorMsg);
    }
    return 0;
}

void nandSettingsDefaults()
{
    gSettings->version = SETTINGS_VER;

    for (int i = 0; i < LOG_OPTION_COUNT; i++)
        gSettings->logOptions[i] = LogType::NONE;

    gSettings->hudMapDoor = false;
    gSettings->hudXYZ = false;
    gSettings->xyzInterval = 4;
    gSettings->xyzDP = 2;
    
    gSettings->mapChangeEffect = true;

    gSettings->customPitText = false;
}

static bool (*isRelLoadedReal)() = nullptr;
static s32 evtId = -1;
bool loadOnBoot()
{
    if (evtId == -1)
    {
        // Start settings load evt script if this is the first run
        evtId = spm::evtmgr::evtEntryType(nand_settings_load, 0, 0, 0)->id;
        wii::OSError::OSReport("nandsettings: starting evt script\n");
    }
    else if ((evtId != -2) && (spm::evtmgr::evtCheckId(evtId) == false))
    {
        // If it didn't succeed, use defaults
        if (!gNandSettingsSuccess)
        {
            wii::OSError::OSReport("nandsettings: using defaults\n");
            nandSettingsDefaults();
        }

        // Signal finishing
        evtId = -2;
        wii::OSError::OSReport("nandsettings: evt script done\n");
    }

    // Check if it's done
    if (evtId == -2)
        // Carry out normal behaviour
        return isRelLoadedReal();
    else
        // Stall game if still loading
        return false;
}

void nandSettingsPatch()
{
    // Hook on the rel loaded check to load settings from NAND
    // (this is the earliest that it can be guaranteed the rel patches have ran)
    isRelLoadedReal = patch::hookFunction(spm::relmgr::isRelLoaded, loadOnBoot);
}

}
