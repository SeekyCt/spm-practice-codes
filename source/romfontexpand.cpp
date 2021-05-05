#include "util.h"

#include <types.h>
#include <spm/romfont.h>
#include <wii/OSCache.h>
#include <wii/OSFont.h>
#include <wii/stdio.h>
#include <wii/string.h>

namespace mod {

using namespace spm::romfont;

char newChars[][2] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "<", ">", "[", "]",
                      "(", ")", "=", "j", "H", "J", "K", "Q", "X", "Y", "Z", "_"};

#define NEW_CHAR_COUNT ARRAY_SIZEOF(newChars)

void romfontExpand()
{
    // Allocate memory to load font
    u8 * fontHeader;
    u8 * temp;
    if (wii::OSFont::OSGetFontEncode() == 1)
    {
        fontHeader = new u8[0x90ee4];
        temp = new u8[0x4d000];
    }
    else
    {
        fontHeader = new u8[0x10120];
        temp = new u8[0x3000];
    }

    // Load font
    wii::OSFont::OSLoadFont(fontHeader, temp);

    // Create new entry array
    int originalCount = romfontWp->entryCount;
    romfontWp->entryCount += NEW_CHAR_COUNT;
    RomfontEntry * newEntries = new RomfontEntry[romfontWp->entryCount];
    wii::string::memcpy(newEntries, romfontWp->entries, sizeof(RomfontEntry) * romfontWp->entryCount);

    // Add new characters
    for (u32 i = 0; i < NEW_CHAR_COUNT; i++)
    {
        int width;
        wii::OSFont::OSGetFontTexel(newChars[i], newEntries[originalCount + i].image, 0, 6, &width);
        newEntries[originalCount + i].character = newChars[i][0];
        newEntries[originalCount + i].width = width;
    }

    // Free font working memory
    delete[] fontHeader;
    delete[] temp;

    // Replace old entry array
    delete[] romfontWp->entries;
    romfontWp->entries = newEntries;

    // Flush cache
    wii::OSCache::DCFlushRange(romfontWp->entries, sizeof(RomfontEntry) * romfontWp->entryCount);
}

}
