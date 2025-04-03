#include "event.h"
#include "searchAlgorithms.h"
#include "sortingAlgorithms.h"

void addEventToFront(EVENT** head, EVENT* newEvent) {
    newEvent->next = *head;
    *head = newEvent;
}

void addEvent(EVENT** head, int& userId, sqlite3* db) {
    static int count = 0;
    if (count == 0) {
        std::cin.ignore();
        count++;
    }

    std::string title, date, info, type;

    while (!getValidInput("Enter Title", title))
        std::cout << "Error: Title cannot be empty.\n";
    while (!getValidInput("Enter Date (DD/MM/YYYY)", date))
        std::cout << "Error: Date cannot be empty.\n";
    while (!getValidInput("Enter Info", info))
        std::cout << "Error: Info cannot be empty.\n";

    // Prompt for type with validation
    bool validType = false;
    while (!validType) {
        while (!getValidInput("Enter Type (War or Revolution)", type))
            std::cout << "Error: Type cannot be empty.\n";
        if (type == "War" || type == "Revolution") {
            validType = true;
        }
        else {
            std::cout << "Error: Type must be either 'War' or 'Revolution'.\n";
        }
    }

    Authentication auth(db);
    bool success = auth.addEvent(userId, title, date, info, type);
    if (!success) {
        std::cout << "Error inserting event into database.\n";
        return;
    }

    EVENT* newEvent = new EVENT{ title, 0, 0, 0, info, nullptr, type };
    formatDate(date, newEvent);

    addEventToFront(head, newEvent);
}

void showEventList(EVENT* head, const std::string& filter) {
    std::cout << "Available events";
    if (!filter.empty())
        std::cout << " (filtered by type: " << filter << ")";
    std::cout << ":\n";

    bool anyDisplayed = false;
    if (head == nullptr) {
        std::cout << "No historical events recorded.\n";
    }
    else {
        EVENT* list = head;
        while (list != nullptr) {
            if (filter.empty() || list->type == filter) {
                std::cout << "- " << list->title << "\n";
                anyDisplayed = true;
            }
            list = list->next;
        }
        if (!anyDisplayed) {
            std::cout << "No events match the current filter.\n";
        }
    }
}

void displayEventInfo(EVENT* head, const std::string& filter) {
    system("cls");
    std::string searchTitle;
    std::cout << "Enter the name of the event to view details: ";
    std::getline(std::cin, searchTitle);
    EVENT* list = head;
    bool found = false;
    while (list != nullptr) {
        if ((filter.empty() || list->type == filter) && list->title == searchTitle) {
            std::cout << "\nDate: " << list->dateDay << "/"
                << list->dateMonth << "/" << list->dateYear << "\n";
            std::cout << "Info: " << list->info << "\n";
            std::cout << "Type: " << list->type << "\n";
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

void displaySortedEvents(EVENT* head, const std::string& filter) {
    system("cls");

    int sortSelection = 0;
    while (true) {
        system("cls");
        std::cout << "Choose sorting order:\n";
        std::cout << (sortSelection == 0 ? "> " : "  ") << "Title (Ascending)\n";
        std::cout << (sortSelection == 1 ? "> " : "  ") << "Title (Descending)\n";
        std::cout << (sortSelection == 2 ? "> " : "  ") << "Date (Ascending)\n";
        std::cout << (sortSelection == 3 ? "> " : "  ") << "Date (Descending)\n";
        std::cout << (sortSelection == 4 ? "> " : "  ") << "Go back\n";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
            case 72:
                if (sortSelection > 0) sortSelection--;
                break;
            case 80:
                if (sortSelection < 4) sortSelection++;
                break;
            }
        }
        else if (key == 13) {
            if (sortSelection == 4) return;

            EVENT* sortedHead = nullptr;
            switch (sortSelection) {
            case 0:
                sortedHead = sortEventsByTitle(head);
                break;
            case 1:
                sortedHead = sortEventsByTitleDescending(head);
                break;
            case 2:
                sortedHead = sortEventsByDate(head);
                break;
            case 3:
                sortedHead = sortEventsByDateDescending(head);
                break;
            }

            system("cls");
            std::cout << "Sorted events";
            if (!filter.empty())
                std::cout << " (filtered by type: " << filter << ")";
            std::cout << ":\n";
            EVENT* curr = sortedHead;
            bool anyDisplayed = false;
            while (curr != nullptr) {
                if (filter.empty() || curr->type == filter) {
                    std::cout << "- " << curr->title << " ("
                        << curr->dateDay << "/"
                        << curr->dateMonth << "/"
                        << curr->dateYear << ")\n";
                    anyDisplayed = true;
                }
                curr = curr->next;
            }
            if (!anyDisplayed)
                std::cout << "No events match the current filter.\n";
            std::cout << "\nPress Enter to return...";
            std::cin.ignore();
            std::cin.get();
            return;
        }
    }
}

void displayEvents(EVENT* head, int& userId, sqlite3* db) {
    std::string currentFilter = ""; // Empty means no filtering (all events shown)

    system("cls");
    showEventList(head, currentFilter);
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();

    int currentSelection = 0;
    while (true) {
        system("cls");
        showEventList(head, currentFilter);

        std::cout << "\nCurrent filter: "
            << (currentFilter.empty() ? "None" : currentFilter)
            << "\n";
        std::cout << "\nUse up/down arrows to navigate, Enter to select\n\n";
        std::cout << (currentSelection == 0 ? "> " : "  ") << "Display more info\n";
        std::cout << (currentSelection == 1 ? "> " : "  ") << "Sort events\n";
        std::cout << (currentSelection == 2 ? "> " : "  ") << "Edit event\n";
        std::cout << (currentSelection == 3 ? "> " : "  ") << "Delete event\n";
        std::cout << (currentSelection == 4 ? "> " : "  ") << "Set filter type\n";
        std::cout << (currentSelection == 5 ? "> " : "  ") << "Clear filter\n";
        std::cout << (currentSelection == 6 ? "> " : "  ") << "Go back to main menu\n";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
            case 72:
                if (currentSelection > 0)
                    currentSelection--;
                break;
            case 80:
                if (currentSelection < 6)
                    currentSelection++;
                break;
            default:
                break;
            }
        }
        else if (key == 13) {
            if (currentSelection == 0) {
                displayEventInfo(head, currentFilter);
            }
            else if (currentSelection == 1) {
                displaySortedEvents(head, currentFilter);
            }
            else if (currentSelection == 2) {
                editEvent(head, userId, db);
                std::cout << "\nEvent edited (if it existed).\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
            }
            else if (currentSelection == 3) {
                std::string title;
                std::cout << "\nEnter the title of the event to delete: ";
                std::getline(std::cin, title);
                deleteEvent(&head, title, userId, db);
                std::cout << "\nEvent deleted (if it existed).\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
            }
            else if (currentSelection == 4) {
                // Set filter type: prompt for "War" or "Revolution"
                std::string type;
                bool validType = false;
                while (!validType) {
                    std::cout << "\nEnter filter type (War or Revolution, leave empty for no filter): ";
                    std::getline(std::cin, type);
                    if (type.empty() || type == "War" || type == "Revolution") {
                        validType = true;
                    }
                    else {
                        std::cout << "Invalid type. Please enter 'War', 'Revolution', or leave empty.\n";
                    }
                }
                currentFilter = type; // If empty, this will show all events.
            }
            else if (currentSelection == 5) {
                currentFilter = "";
            }
            else if (currentSelection == 6) {
                break;
            }
        }
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

void editEvent(EVENT* head, int userId, sqlite3* db) {
    std::cout << "Enter the title of the event you want to edit: ";
    std::string title;
    std::getline(std::cin, title);
    Authentication auth(db);
    EVENT* event = head;
    while (event != nullptr) {
        if (event->title == title) {
            std::string newTitle, newDate, newInfo, newType;

            std::cout << "Enter new title (leave empty to keep current): ";
            std::getline(std::cin, newTitle);
            if (newTitle.empty()) {
                newTitle = event->title;
            }

            std::cout << "Enter new date (leave empty to keep current): ";
            std::getline(std::cin, newDate);
            if (newDate.empty()) {
                newDate = std::to_string(event->dateDay) + "/" +
                    std::to_string(event->dateMonth) + "/" +
                    std::to_string(event->dateYear);
            }

            std::cout << "Enter new info (leave empty to keep current): ";
            std::getline(std::cin, newInfo);
            if (newInfo.empty()) {
                newInfo = event->info;
            }

            bool validType = false;
            while (!validType) {
                std::cout << "Enter new type (War or Revolution, leave empty to keep current): ";
                std::getline(std::cin, newType);
                if (newType.empty()) {
                    newType = event->type;
                    validType = true;
                }
                else if (newType == "War" || newType == "Revolution") {
                    validType = true;
                }
                else {
                    std::cout << "Invalid type. Please enter 'War' or 'Revolution'.\n";
                }
            }

            if (auth.updateEvent(userId, event->title, newTitle, newDate, newInfo, newType)) {
                event->title = newTitle;
                formatDate(newDate, event);
                event->info = newInfo;
                event->type = newType;
                std::cout << "Event updated successfully.\n";
            }
            else {
                std::cout << "Error updating event in database.\n";
            }
            return;
        }
        event = event->next;
    }

    std::cout << "Event not found.\n";
}
