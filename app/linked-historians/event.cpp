#include "event.h"
#include "searchAlgorithms.h"
#include "sortingAlgorithms.h" 

static bool getValidInput(const std::string& prompt, std::string& input) {
    std::cout << prompt;
    std::getline(std::cin, input);
    return !input.empty() && input.find_first_not_of(" \t\n\r") != std::string::npos;
}

void addEvent(EVENT** head, int& userId, sqlite3* db) {
    static int count = 0;
    if (count == 0) {
        std::cin.ignore();
        count++;
    }

    std::string title, date, info;

    while (!getValidInput("Enter Title: ", title)) std::cout << "Error: Title cannot be empty.\n";
    while (!getValidInput("Enter Date: ", date)) std::cout << "Error: Date cannot be empty.\n";
    while (!getValidInput("Enter Info: ", info)) std::cout << "Error: Info cannot be empty.\n";

    Authentication auth(db);
    bool success = auth.addEvent(userId, title, date, info);
    if (!success) {
        std::cout << "Error inserting event into database.\n";
        return;
    }

    EVENT* newEvent = new EVENT{ title, date, info, nullptr };
    if (*head == nullptr) {
        *head = newEvent;
    }
    else {
        EVENT* temp = *head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newEvent;
    }

}


void showEventList(EVENT* head) {
    std::cout << "Available events:\n";
    if (head == nullptr) {
        std::cout << "No historical events recorded.\n";
    }
    else {
        EVENT* list = head;
        while (list != nullptr) {
            std::cout << "- " << list->title << "\n";
            list = list->next;
        }
    }
}

void displayEventInfo(EVENT* head) {
    system("cls");
    std::string searchTitle;
    std::cout << "Enter the name of the event to view details: ";
    std::getline(std::cin, searchTitle);
    EVENT* list = head;
    bool found = false;
    while (list != nullptr) {
        if (list->title == searchTitle) {
            std::cout << "\nInfo: " << list->info << "\n";
            found = true;
            break;
        }
        list = list->next;
    }
    if (!found)
        std::cout << "\nEvent not found.\n";

    std::cout << "\nPress Enter to return to the submenu...";
    std::cin.ignore();
    std::cin.get();
}

void displaySortedEvents(EVENT* head) {
    system("cls");

    int sortSelection = 0;
    while (true) {
        system("cls");
        std::cout << "Choose sorting order:\n";
        std::cout << (sortSelection == 0 ? "> " : "  ") << "Ascending\n";
        std::cout << (sortSelection == 1 ? "> " : "  ") << "Descending\n";
        std::cout << (sortSelection == 2 ? "> " : "  ") << "Go back\n";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
            case 72: 
                if (sortSelection > 0) sortSelection--;
                break;
            case 80: 
                if (sortSelection < 2) sortSelection++;
                break;
            }
        }
        else if (key == 13) { 
            if (sortSelection == 2) return;

            EVENT* sortedHead = (sortSelection == 0) ? sortEventsByTitle(head) : sortEventsByTitleDescending(head);
            system("cls");
            std::cout << "Sorted events:\n";
            EVENT* curr = sortedHead;
            while (curr != nullptr) {
                std::cout << "- " << curr->title << "\n";
                curr = curr->next;
            }
            std::cout << "\nPress Enter to return...";
            std::cin.ignore();
            std::cin.get();
            return;
        }
    }
}

void displayEvents(EVENT* head, int& userId, sqlite3* db)
{
    system("cls");
    showEventList(head);
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();

    int currentSelection = 0;
    while (true)
    {
        system("cls");
        showEventList(head);

        std::cout << "\nUse up/down arrows to navigate, Enter to select\n\n";
        std::cout << (currentSelection == 0 ? "> " : "  ") << "Display more info\n";
        std::cout << (currentSelection == 1 ? "> " : "  ") << "Sort events\n";
        std::cout << (currentSelection == 2 ? "> " : "  ") << "Delete event\n";
        std::cout << (currentSelection == 3 ? "> " : "  ") << "Go back to main menu\n";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
            case 72:
                if (currentSelection > 0)
                    currentSelection--;
                break;
            case 80:
                if (currentSelection < 3)
                    currentSelection++;
                break;
            default:
                break;
            }
        }
        else if (key == 13) {
            if (currentSelection == 0) {
                displayEventInfo(head);
            }
            else if (currentSelection == 1) {
                displaySortedEvents(head);
            }
            else if (currentSelection == 2) {
                std::string title;
                std::cout << "\nEnter the title of the event to delete: ";
                std::getline(std::cin, title);
                deleteEvent(&head, title, userId, db);
                std::cout << "\nEvent deleted (if it existed).\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
            }
            else if (currentSelection == 3) {
                break;
            }
        }
    }
}

void searchInEvent(EVENT* head, const std::string& searchKeyword) {
    EVENT* list = head;
    bool eventFound = false;
    bool keywordFound = false;

    std::cout << "Available events: \n";
    if (list == nullptr) {
        std::cout << "No historical events recorded.\n";
        return;
    }

    while (list != nullptr) {
        std::cout << "- " << list->title << "\n";
        list = list->next;
    }

    list = head;
    std::cout << "\nEnter the name of the event to view details: ";
    std::string eventTitle;
    std::getline(std::cin, eventTitle);

    while (list != nullptr) {
        if (list->title == eventTitle) {
            eventFound = true;
            std::cout << "\nEnter a keyword to search in the info: ";
            std::string keyword;
            std::getline(std::cin, keyword);

            if (containsSubstring(list->info, keyword)) {
                std::cout << "Keyword found in the event info!\n";
                std::cout << "Info: " << list->info << "\n";
                keywordFound = true;
                std::cout << "\n";
            }

            if (!keywordFound) {
                std::cout << "Keyword not found in the event's info.\n";
                std::cin.get();
            }
            else
            {
                std::cin.get();
            }
            break;
        }
        list = list->next;

    }

    if (!eventFound) {
        std::cout << "Event not found.\n";
    }

}

void deleteEvent(EVENT** head, std::string& title, int userId, sqlite3* db) {
    Authentication auth(db);
    if (!auth.deleteEvent(userId, title)) {
        std::cout << "Error deleting event from database.\n";
        return;
    }

    EVENT* temp = *head;
    EVENT* prev = nullptr;
    if (temp != nullptr && temp->title == title) {
        *head = temp->next;
        delete temp;
        return;
    }
    while (temp != nullptr && temp->title != title) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == nullptr)
        return;
    prev->next = temp->next;
    delete temp;
}
