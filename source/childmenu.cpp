#include "childmenu.h"
#include "mainmenu.h"

namespace mod {

void ChildMenu::close()
{
    // Change back to parent menu
    delete MenuWindow::sCurMenu;
    MenuWindow::sCurMenu = new MainMenu();
}

}