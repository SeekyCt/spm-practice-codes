/*
    HPWindow
    Enemy health displays
*/
#pragma once

#include <common.h>
#include <spm/npcdrv.h>

#include "mod_ui_base/window.h"

namespace mod {

class HPWindow final : public Window
{
private:
    static const s32 bossTribes[];
    static const s32 blacklistedTribes[];

    /*
        Checks if an NPC is a boss
    */
    bool npcBossTribeCheck(spm::npcdrv::NPCEntry * npc);

    /*
        Checks if an NPC is an enemy
    */
    bool npcEnemyTribeCheck(spm::npcdrv::NPCEntry * npc);

    /*
        If a door has been entered in this map, checks that the NPC is shown by it
    */
    bool npcDoorCheck(const char * name);

    /*
        Checks that an NPC isn't behind the camera in 3d
    */
    bool npcPosCheck(spm::npcdrv::NPCEntry * npc);

    /*
        Returns the current/max HP of a boss, with special cases for ones not using standard HP
    */
    s32 bossGetHp(spm::npcdrv::NPCEntry * npc);
    s32 bossGetMaxHp(spm::npcdrv::NPCEntry * npc);

    /*
        The big blooper boss consists of multiple NPCs with the same tribe id, only one stores
        its health information
    */
    bool npcBossBlooperCheck(spm::npcdrv::NPCEntry * npc);

    /*
        Returns if a Sammer Guy is actually fighting
    */
    bool npcBossSammerCheck(spm::npcdrv::NPCEntry * npc);

    /*
        Draws the boss HP bar at the top of the screen if a boss is active
    */
    void bossDisp();

    /*
        Draw HP bars under enemies
    */
    void enemyDisp();

protected:
    virtual void disp() override;
    HPWindow();

public:
    static HPWindow * sInstance;

    static void init();
};

}
