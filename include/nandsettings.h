/*
    Handles saving settings to and loading settings from NAND
*/

#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/evtmgr.h>

#include "scriptlog.h"

namespace mod {

#define SETTINGS_VER 4
#define SETTINGS_FILE_NAME "pcset.bin"

enum class LogType : u8
{
    NONE,
    OSREPORT,
    SCREEN // (+ OSReport)
};

enum LogOptions
{
    OPTION_SCRIPT_LOG = 0, // scriptlog.cpp
    OPTION_EVT_VAR_LOG = 1, // scriptvarlog.cpp
    OPTION_DEBUG_INSTR = 2 // evtdebug.cpp
};

#define LOG_OPTION_COUNT 3

// Old versions for upgrading
struct NandSettingsV1
{
    u32 version;
    LogType logOptions[3]; 
    bool hudOptions[2];
    bool mapChangeEffect;
};
struct NandSettingsV2
{
    u32 version;

    LogType logOptions[3]; 

    bool hudMapDoor;
    bool hudXYZ;    
    s8 xyzInterval;
    s8 xyzDP;

    bool mapChangeEffect;
};

// Latest
struct NandSettings
{
    u32 version;

    LogType logOptions[LOG_OPTION_COUNT]; 

    bool hudMapDoor;
    bool hudXYZ;    
    s8 xyzInterval;
    s8 xyzDP;

    bool mapChangeEffect;

    bool customPitText;

    bool enemyHpBars;
    bool hpBarHide3d;
    bool bossHpBars;
    bool hitboxNpc;
    bool hitboxNpcDisabled;
    bool lastKey;
    bool inputDisplay;
};

extern NandSettings * gSettings;

EVT_DECLARE(nand_settings_load)
EVT_DECLARE(nand_settings_write)
EVT_DECLARE(nand_settings_delete)

extern int gNandSettingsSuccess;

void nandSettingsDefaults();
void nandSettingsPatch();

}
