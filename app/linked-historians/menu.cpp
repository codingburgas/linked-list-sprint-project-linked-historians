#include "menu.h"
#include "event.h"

void displayOptions(int currentSelection) {
    system("cls");
    std::cout << "Use up/down arrows to navigate, Enter to select, 9 to Exit\n\n";

    std::cout << (currentSelection == 0 ? "> " : "  ") << "1. Add an event\n";
    std::cout << (currentSelection == 1 ? "> " : "  ") << "2. List all events\n";
    std::cout << (currentSelection == 2 ? "> " : "  ") << "3. Search a text in an event\n";
    std::cout << (currentSelection == 3 ? "> " : "  ") << "9. Back to Login\n";
}

void displayMenu(int& id, Authentication& auth) {
    system("cls");
    EVENT* head = nullptr;
    std::string s;
    fetchEvents(&head, id, auth.getDb());

    int currentSelection = 0;
    while (true) {
        displayOptions(currentSelection);
        int key = _getch();

        if (key == 224) {
            key = _getch(); 
            switch (key) {
            case 72: 
                if (currentSelection > 0) currentSelection--;
                break;
            case 80: 
                if (currentSelection < 3) currentSelection++;
                break;
            }
        }
        else if (key == 13) { 
            switch (currentSelection) {
            case 0: addEvent(&head, id, auth.getDb()); break;
            case 1: displayEvents(head); break;
            case 2: searchInEvent(head, s); break;
            case 3: return;
            }
        }
        else if (key == '9') { 
            return;
        }
    }
}