#include <common.h>
#include <spm/msgdrv.h>
#include <spm/system.h>
#include <msl/string.h>

#include "msgrand.h"
#include "patch.h"

namespace mod {

static const char * (*msgSearchReal)(const char * name);
static const char * (*msgSearchNoFallbackReal)(const char * name);

/*
    Get a basic hash of a string's characters
*/
static u32 hashStr(const char * str)
{
    u32 hash = 0;
    while (*str != 0)
        hash = (hash * 37) + *str++;
    return hash;
}

/*
    Checks if a message uses a <k> tag
*/
static bool hasInputWait(const char * str)
{
    return msl::string::strstr(str, "<k>") != nullptr;
}

/*
    Checks whether a message is safe to be randomised
*/
static bool isUseable(const char * str)
{
    return (
        msl::string::strstr(str, "<select") == nullptr &&
        msl::string::strstr(str, "<o>") == nullptr &&
        msl::string::strstr(str, "<once_stop>") == nullptr &&
        msl::string::strstr(str, "<!") == nullptr &&
        msl::string::strstr(str, "%s") == nullptr
    );
}

/*
    Get a random message seeded by a message name
*/
static const char * randomMsg(const char * name)
{
    // Fallback to null if this doesn't exist or is part of a selection
    const char * original = msgSearchNoFallbackReal(name);
    if (original == nullptr)
        return nullptr;
    if (!isUseable(original))
        return nullptr;

    // Find all active message files
    s32 fileIds[MSG_FILE_MAX];
    s32 fileCount = 0;
    for (s32 i = 0; i < MSG_FILE_MAX; i++)
    {
        spm::msgdrv::MsgFile * file = spm::msgdrv::msgdrv_msgw->files + i;
        if (file->messages != nullptr)
        {
            fileIds[fileCount++] = i;
        }
    }

    // Fallback to null if no files found
    if (fileCount == 0)
        return nullptr;
    
    // Select a random file
    s32 fileId = fileIds[hashStr(name) % fileCount];
    spm::msgdrv::MsgFile * file = spm::msgdrv::msgdrv_msgw->files + fileId;

    // Select a random message from the file
    spm::msgdrv::MsgEntry * entries = (spm::msgdrv::MsgEntry *) file->messages->data;
    bool inputWait = hasInputWait(original);
    u32 hash = hashStr(name);
    const char * ret;
    do
    {
        spm::msgdrv::MsgEntry * entry = entries + (hash % file->messageCount);
        ret = file->contents + entry->contentsOffset;
        hash *= 37;
    }
    while (!isUseable(ret) || hasInputWait(ret) != inputWait);
    return ret;
}

void msgRandPatch()
{
    msgSearchReal = patch::hookFunction(spm::msgdrv::msgSearch,
        [](const char * name)
        {
            const char * ret = randomMsg(name);
            if (ret != nullptr)
                return ret;
            else
                return msgSearchReal(name);
        }
    );

    msgSearchNoFallbackReal = patch::hookFunction(spm::msgdrv::msgSearchNoFallback,
        [](const char * name)
        {
            const char * ret = randomMsg(name);
            if (ret != nullptr)
                return ret;
            else
                return msgSearchNoFallbackReal(name);
        }
    );
}

}
