#pragma once

#include <types.h>

namespace spm::mario_pouch {

struct PouchCharOrPixlInfo
{
    bool selectable;
    bool selected;
    int16_t itemType;
};
static_assert(sizeof(PouchCharOrPixlInfo) == 0x4);

struct PouchMinigameScore
{
    int characterId;
    int score;
    u8 unknown_0x8[0x18 - 0x8];
};
static_assert(sizeof(PouchMinigameScore) == 0x18);

struct MarioPouchWork
{
    u8 unknown_0x0[0x4 - 0x0]; // unused?
    int level;
    int attack;
    int hp;
    int maxHp;
    int flipTimer;
    int xp;
    int coins;
    s16 keyItem[32];
    s16 useItem[10];
    s16 shopItem[32];
    PouchCharOrPixlInfo characters[4];
    PouchCharOrPixlInfo pixls[16];
    int shopPoints;
    u32 shopPointRewardsCollected; // bit mask, index 1 << i
    s8 catchCards[256]; // value is count, index is id - first card id
    u8 unknown_0x20c[256];
    u32 ownedMaps[2]; // [id / 32] & 1 << (id % 32)
    u32 foundMaps[2]; // [id / 32] & 1 << (id % 32)
    u32 knownRecipes[3]; // [id / 32] & 1 << (id % 32)
    u32 knownCards[8]; // [id / 32] & 1 << (id % 32)
    int charmsRemaining;
    int killsBeforeNextCharm;
    u8 unknown_0x350[0x368 - 0x350];
    PouchMinigameScore minigameScores[4][5];
    u8 unknown_0x548[0x688 - 0x548];
    int arcadeTokens;
    int totalCoinsCollected;
    int maxJumpCombo;
    int maxStylishCombo;
    int enemiesDefeated;
    u8 unknown_0x69c[0x6a0 - 0x69c]; // unused?
};
static_assert(sizeof(MarioPouchWork) == 0x6a0);

extern "C" {

MarioPouchWork * pouchGetPtr();
int pouchGetHp();
void pouchAddHp(int increase);
int pouchGetXp();
void pouchAddXp(int increase);

bool pouchAddItem(int itemId);
bool pouchCheckHaveItem(s16 itemId);
void pouchRemoveItem(int itemId);
void pouchRemoveShopItem(int itemId);

int pouchGetCurPixl();

int pouchCountUseItems();
int pouchCountKeyItems();
int pouchCountShopItems();
int pouchCountChars();
int pouchCountPixls();

}

}

