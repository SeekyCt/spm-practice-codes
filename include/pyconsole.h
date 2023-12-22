/*
    PyConsole
    Code to handle inputs from a python console using the dolphin_memory_engine module 
*/
#pragma once

#ifdef PYCONSOLE_PROTOTYPE
#include <common.h>

namespace mod {

enum class PyConsoleErr
{
    NONE,
    UNK_CMD,
    BAD_ARGS,
    BAD_ARG_COUNT
};

typedef PyConsoleErr (PyCommandCb)(s32 argc, const char ** argv);

class PyCommand
{
private:
    static PyCommand * sCommandList;
    static PyConsoleErr runCommand(const char * command, s32 argc, const char ** argv);

    PyCommand * mNext;
    const char * mName;
    size_t mNameLen;
    PyCommandCb * mCb;

    PyCommand(const char * name, PyCommandCb * cb);

    friend class PyConsole;

public:
    static void add(const char * name, PyCommandCb * cb);
};

#define PY_CONSOLE_BUF_SIZE 256

// Must be synced with python code
#define PY_CONSOLE_PARAM_PTR_ADDR 0x800001a0

// Offsets must be synced with python code
struct PyConsoleParams
{
    char * buffer; // 0x0
    size_t bufferSize; // 0x4
};

class PyConsole
{
private:
    static PyConsoleParams sParams;

public:
    /*
        Creates buffer and outputs information for python to read
    */
    static void init();

    /*
        Handles inputs
    */
    static void main();
};

}
#endif
