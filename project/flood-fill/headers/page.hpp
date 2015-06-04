#ifndef PAGE_H
#define PAGE_H

#include <map>
#include <string>

#include "menu_item.hpp"
#include "level_button.hpp"

#include "pointer_macros.h"
DEF_PTR(Page, PagePtr);

class Page {
    
public:

    Page();

    void addToPageButton(std::string name, MenuItemPtr menuItem);
    void addToPageLevel(MenuItemPtr levelButton);
    void addToPageInertText(std::string name, TextPtr text);
    void removeFromPageButton(std::string name);
    void changeLevel(int direction);
    void display();
    void hide();
    void update();

private:
    int lvlndx;
    int numLevels;
    std::map< std::string, TextPtr > inertText;
    std::map< std::string, MenuItemPtr > menuItems;
    std::map< int, MenuItemPtr > levels;
};

#endif
