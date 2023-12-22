#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/relmgr.h>
#include <spm/system.h>
#include <wii/nand.h>
#include <wii/os.h>
#include <msl/stdio.h>
#include <msl/string.h>

#include "evt_cmd.h"
#include "evtnandapi.h"
#include "nandsettings.h"
#include "patch.h"
#include "util.h"

namespace mod {

// Data to be passed into NAND lib
static u8 _settings[ROUND_UP_32(sizeof(NandSettings))] ALIGNED(NAND_ALIGN);
static u8 openBuf[0x4000] ALIGNED(NAND_ALIGN);
static wii::nand::NANDFileInfo fileInfo;
static wii::nand::NANDCommandBlock commandBlock;

// Actual settings struct
NandSettings * gSettings = reinterpret_cast<NandSettings *>(&_settings);

// Feedback for script callers
int gNandSettingsSuccess;

// evt_nandsettings_handle_read_output()
EVT_DECLARE_USER_FUNC(evt_nandsettings_handle_read_output, 0)
// evt_nandsettings_fail(ret)
EVT_DECLARE_USER_FUNC(evt_nandsettings_fail, 1)

EVT_BEGIN(nand_settings_load)
    // Try open settings file
    USER_FUNC(evt_nand_open, PTR(SETTINGS_FILE_NAME), PTR(&fileInfo), NAND_MODE_READ,
              PTR(openBuf), sizeof(openBuf), PTR(&commandBlock), LW(0))

    IF_EQUAL(LW(0), NAND_CODE_OK)
        // If file opened, read it
        USER_FUNC(evt_nand_read, PTR(&fileInfo), PTR(_settings), sizeof(_settings),
                  PTR(&commandBlock), LW(0))
        USER_FUNC(evt_nandsettings_handle_read_output)
        IF_LARGE_EQUAL(LW(0), 0)
            // Clean up if successful
            USER_FUNC(evt_nand_close, PTR(&fileInfo), PTR(&commandBlock), LW(0))
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
    USER_FUNC(evt_nandsettings_fail, LW(0))
EVT_END()

#define retry 1
EVT_BEGIN(nand_settings_write)
    LBL(retry)

    // Try open settings file
    USER_FUNC(evt_nand_open, PTR(SETTINGS_FILE_NAME), PTR(&fileInfo), NAND_MODE_WRITE,
              PTR(openBuf), sizeof(openBuf), PTR(&commandBlock), LW(0))

    // Create file if needed
    IF_EQUAL(LW(0), NAND_CODE_NOT_EXIST)
        // Check creating it is possible
        USER_FUNC(evt_nand_check, BYTES_TO_BLOCKS(sizeof(_settings)), 1,
                  PTR(&commandBlock), LW(0))

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
        USER_FUNC(evt_nand_write, PTR(&fileInfo), PTR(_settings), sizeof(_settings),
                  PTR(&commandBlock), LW(0))

        IF_EQUAL(LW(0), (s32)sizeof(_settings))
            // Clean up if successful
            USER_FUNC(evt_nand_close, PTR(&fileInfo), PTR(&commandBlock), LW(0))
            IF_EQUAL(LW(0), 0)
                SET_RAM(1, PTR(&gNandSettingsSuccess))
                RETURN()
            END_IF()
        END_IF()
    END_IF()

    // If an unhandled NAND error happened, kill the game
    USER_FUNC(evt_nandsettings_fail, LW(0))
EVT_END()
#undef retry

EVT_BEGIN(nand_settings_delete)
    // Try open settings file to check it exists
    USER_FUNC(evt_nand_open, PTR(SETTINGS_FILE_NAME), PTR(&fileInfo), NAND_MODE_READ,
              PTR(openBuf), sizeof(openBuf), PTR(&commandBlock), LW(0))

    IF_EQUAL(LW(0), NAND_CODE_OK)
        // Try close file
        USER_FUNC(evt_nand_close, PTR(&fileInfo), PTR(&commandBlock), LW(0))

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
    USER_FUNC(evt_nandsettings_fail, LW(0))
EVT_END()

s32 evt_nandsettings_handle_read_output(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    (void) entry;
    (void) firstRun;

    // Dump contents for debugging
    wii::os::OSReport("nandsettings: read at %x, contents:\n", (u32) &_settings);
    u32 * d = reinterpret_cast<u32 *>(gSettings);
    for (u32 i = 0; i < (sizeof(*gSettings) / sizeof(u32)); i += 4)
        wii::os::OSReport("%08x %08x %08x %08x\n", d[i], d[i+1], d[i+2], d[i+3]);

    // Handle settings version
    switch (gSettings->version)
    {
        case 1:
            NandSettingsV1 v1;
            msl::string::memcpy(&v1, gSettings, sizeof(v1));
            wii::os::OSReport("nandsettings: updating settings v1->2.\n");

            // Move relocated settings
            // hudMapDoor and hudXYZ are already in place
            gSettings->mapChangeEffect = v1.mapChangeEffect;

            // Initialise new settings
            gSettings->xyzInterval = 4;
            gSettings->xyzDP = 2;

            // Increment version
            gSettings->version = 2;

            // Fall through into v2->3
            [[fallthrough]];

        case 2:
            wii::os::OSReport("nandsettings: updating settings v2->3.\n");

            // Existing settings are already in place

            // Initialise new setting
            gSettings->customPitText = false;

            // Increment version
            gSettings->version = 3;

            // Fall through into v3->4
            [[fallthrough]];

        case 3:
            wii::os::OSReport("nandsettings: updating settings v3->4\n");

            // Existing settings are already in place

            // Initialise new settings
            gSettings->enemyHpBars = false;
            gSettings->hpBarHide3d = false;
            gSettings->bossHpBars = false;
            gSettings->hitboxNpc = false;
            gSettings->hitboxNpcDisabled = false;
            gSettings->lastKey = false;
            gSettings->inputDisplay = false;

            // Increment version
            gSettings->version = 4;

            break;

        case SETTINGS_VER:
            wii::os::OSReport("nandsettings: settings version ok.\n");
            break;

        default:
            wii::os::OSReport("nandsettings: settings file too new, using defaults\n");
            nandSettingsDefaults();
            break;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_nandsettings_fail(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    if (firstRun)
    {
        assertf(0, "SPM Practice Codes NAND error %d\n(please report this)",
                spm::evtmgr_cmd::evtGetValue(entry, entry->pCurData[0]));
    }
    return EVT_RET_BLOCK_WEAK;
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

    gSettings->enemyHpBars = false;
    gSettings->hpBarHide3d = false;
    gSettings->bossHpBars = false;
    gSettings->hitboxNpc = false;
    gSettings->hitboxNpcDisabled = false;
    gSettings->lastKey = false;
    gSettings->inputDisplay = false;
}

static bool (*isRelLoadedReal)() = nullptr;
static s32 evtId = -1;
bool loadOnBoot()
{
    if (evtId == -1)
    {
        // Start settings load evt script if this is the first run
        evtId = spm::evtmgr::evtEntryType(nand_settings_load, 0, 0, 0)->id;
        wii::os::OSReport("nandsettings: starting evt script\n");
    }
    else if ((evtId != -2) && (spm::evtmgr::evtCheckID(evtId) == false))
    {
        // If it didn't succeed, use defaults
        if (!gNandSettingsSuccess)
        {
            wii::os::OSReport("nandsettings: using defaults\n");
            nandSettingsDefaults();
        }

        // Signal finishing
        evtId = -2;
        wii::os::OSReport("nandsettings: evt script done\n");
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
