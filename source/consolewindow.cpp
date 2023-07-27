#include <common.h>
#include <wii/os.h>
#include <msl/string.h>

#include "mod_ui_base/colours.h"
#include "mod_ui_base/menuwindow.h"
#include "consolewindow.h"
#include "patch.h"
#include "util.h"
#include "ug.h"

namespace mod {

ConsoleWindow * ConsoleWindow::sInstance = nullptr;

ConsoleWindow::ConsoleWindow()
{
    // Initialise properties
    mPosX = -365.0f;
    mPosY = -128.0f;
    mScale = 0.6f;
    mFadeThreshhold = 80;
    mLineLifetime = 450;
}

void ConsoleWindow::push(const char * text, const wii::gx::GXColor * colour, ConsoleFreeCallback * cb)
{
    sInstance->_push(text, colour, cb);
}

void ConsoleWindow::pushClone(const char * text, const wii::gx::GXColor * colour)
{
    sInstance->_push(cloneString(text), colour, autoFreeCb);
}

void ConsoleWindow::_push(const char * text, const wii::gx::GXColor * colour, ConsoleFreeCallback * cb)
{
    // Print to OSReport
    wii::os::OSReport("(Console %p) %s\n", this, text);

#ifdef PYCONSOLE_PROTOTYPE
    // Print to USB Gecko
    if (ugProbe(1))
        ugSend(1, text, msl::string::strlen(text)), ugSend(1, "\n", 1);
#endif

    // Handle colour
    wii::gx::GXColor _colour;
    if (colour != nullptr)
        _colour = *colour, _colour.a = 0xff; // override alpha for timed fade effect
    else
        _colour = colours::green;

    // Append to list of active console lines for mLineLifetime frames
    mLines = new ConsoleLine {text, _colour, mLineLifetime, mLines, cb};
}

void ConsoleWindow::autoFreeCb(const char * line)
{
    delete[] line;
}

void ConsoleWindow::disp()
{
    // Don't draw over menu
    if (MenuWindow::sCurMenu != nullptr)
        return;

    // Initialise loop vars
    ConsoleLine *line = mLines;
    ConsoleLine *prevLine = nullptr;
    f32 y = mPosY;

    // Draw and update all lines
    while (line != nullptr)
    {
        // Decrease alpha if lifetime within fading region
        if (line->lifetime <= mFadeThreshhold)
            line->colour.a -= 0xff / mFadeThreshhold;
        
        // Draw line if still on screen
        if (y < 220.0f)
            drawString(line->line, mPosX, y, &line->colour, mScale, true);

        // Increment y for next line
        y += mScale * (FONT_HEIGHT + 5.0f);

        // Decrement life timer, free if last item
        if (line->lifetime-- == 0)
        {
            // Keep a temporary copy of line to free
            ConsoleLine *temp = line;
            
            // Move to next line
            line = line->next;

            // Call free callback on this line if needed
            if (temp->freeCallback != nullptr)
                temp->freeCallback(temp->line);

            // Free this line
            delete temp;

            if (prevLine != nullptr)
                // Replace previous item in list's next pointer
                prevLine->next = line;
            else
                // This was the first item in the list, update start pointer
                mLines = line;
        }
        else 
        {
            // Move to next line
            prevLine = line;
            line = line->next;
        }
    }
}

void ConsoleWindow::init()
{
    sInstance = new ConsoleWindow();
}

}
