#include "mod_ui_base/colours.h"
#include "consolewindow.h"
#include "pyconsole.h"
#include "util.h"
#include "ug.h"

#include <types.h>
#include <wii/OSError.h>
#include <wii/string.h>

namespace mod {

PyCommand * PyCommand::sCommandList = nullptr;
PyConsoleParams PyConsole::sParams;

PyConsoleErr PyCommand::runCommand(const char * command, s32 argc, const char ** argv)
{
    // Find command
    size_t len = wii::string::strlen(command);
    for (PyCommand * p = sCommandList; p; p = p->mNext)
    {
        if (len == p->mNameLen && wii::string::strncmp(command, p->mName, p->mNameLen) == 0)
        {
            // Run command and return its output
            return p->mCb(argc, argv);
        }
    }

    // Error if not found
    return PyConsoleErr::UNK_CMD;
}

PyCommand::PyCommand(const char * name, PyCommandCb * cb)
{
    // Append to linked list
    mNext = sCommandList;
    sCommandList = this;

    // Store params
    mName = name;
    mCb = cb;

    // Pre-calculate length
    mNameLen = wii::string::strlen(name);
}

void PyCommand::add(const char * name, PyCommandCb * cb)
{
    // Create a PyCommand with this info
    new PyCommand(name, cb);
}

void PyConsole::init()
{
    // Allocate buffer and place info where python program can find it
    sParams.buffer = new char[PY_CONSOLE_BUF_SIZE];
    sParams.bufferSize = PY_CONSOLE_BUF_SIZE;
    *(PyConsoleParams **)PY_CONSOLE_PARAM_PTR_ADDR = &sParams;
}

void PyConsole::main()
{
    // Also check for commands from a USB Gecko
    if (sParams.buffer[0] == 0 && ugProbe(1))
    {
        s32 len = ugRecv(1, sParams.buffer, sParams.bufferSize);
        sParams.buffer[len - 1] = 0;
    }

    // Check for commands
    if (sParams.buffer[0] != 0)
    {
        char * command = sParams.buffer;

        // Write  command onto screen
        ConsoleWindow::pushClone(command);

        // Prepare arguments
        s32 argc = strcount(command, ' ');
        const char ** argv;
        if (argc > 0)
            argv = new const char * [argc];
        else
            argv = nullptr;
        s32 idx = 0;
        for (s32 i = 0; i < argc; i++)
        {
            while (command[idx] != ' ' && command[idx] != 0)
                idx++;

            command[idx] = 0;
            argv[i] = command + ++idx;
        }

        // Try to run commmand
        PyConsoleErr ret = PyCommand::runCommand(command, argc, argv);
        
        // Handle result
        switch(ret)
        {
            case PyConsoleErr::NONE:
                break;

            case PyConsoleErr::UNK_CMD:
                ConsoleWindow::push("Error: unknown command", &colours::red);
                break;
            
            case PyConsoleErr::BAD_ARGS:
                ConsoleWindow::push("Error: invalid arguments", &colours::red);
                break;
            
            case PyConsoleErr::BAD_ARG_COUNT:
                ConsoleWindow::push("Error: invalid number of arguments", &colours::red);
                break;
        }
        
        // Clear buffer
        sParams.buffer[0] = 0;
    }
}

}
