#include "menu.h"
#include "event.h"
#include "searchAlgorithms.h"
#include "utilities.h"
#include "authentication.h"

void displayOptions(int currentSelection) {
    system("cls");
    std::cout << "Use up/down arrows to navigate, Enter to select, 9 to Exit\n\n";

    std::cout << (currentSelection == 0 ? "> " : "  ") << "1. Add an event\n";
    std::cout << (currentSelection == 1 ? "> " : "  ") << "2. List all events\n";
    std::cout << (currentSelection == 2 ? "> " : "  ") << "9. Back to Login\n";
}

static void deleteEventList(EVENT* head) {
    EVENT* current = head;
    while (current != nullptr) {
        EVENT* next = current->next;  
        delete current;               
        current = next;               
    }
}

void displayMenu(int& id, Authentication& auth) {
    system("cls");
    EVENT* head = nullptr;
    //std::string s;
    auth.fetchEvents(id,&head);

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
                if (currentSelection < 2) currentSelection++;
                break;
            }
        }
        else if (key == 13) { 
            switch (currentSelection) {
            case 0: addEvent(&head, id, auth.getDb()); break;
            case 1: 
                if (head == nullptr || head == reinterpret_cast<EVENT*>(0xFFFFFFFFFFFFFFE7)) {
                    std::cout << "No events available.\n";
                    utilities::waitForEnter();
                    break;
                }
                else {
                    
                    auth.fetchEvents(id, &head);
                    displayEvents(head, id, auth.getDb());
                    break;
                }
            //case 2: searchInEvent(head, s); break;
            case 2: return;
            }
        }
        else if (key == '9') { 
            return;
        }
    }
    if (head != nullptr) {
        deleteEventList(head);
    }
}