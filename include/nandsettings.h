/*
    Handles saving settings to and loading settings from NAND
*/

#pragma once

#include "scriptlog.h"

#include <spm/evtmgr.h>

namespace mod {

#define SETTINGS_VER 2
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
};

extern NandSettings * settings;

EVT_DECLARE(nand_settings_load);
EVT_DECLARE(nand_settings_write);
EVT_DECLARE(nand_settings_delete);

extern int nandSettingsSuccess;

void nandSettingsDefaults();
void nandSettingsPatch();

}
