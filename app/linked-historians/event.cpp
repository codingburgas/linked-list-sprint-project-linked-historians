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

    std::string title, date, info;

    while (!getValidInput("Enter Title", title)) std::cout << "Error: Title cannot be empty.\n";
    while (!getValidInput("Enter Date (DD/MM/YYYY)", date)) std::cout << "Error: Date cannot be empty.\n";
    while (!getValidInput("Enter Info", info)) std::cout << "Error: Info cannot be empty.\n";

    Authentication auth(db);
    bool success = auth.addEvent(userId, title, date, info);
    if (!success) {
        std::cout << "Error inserting event into database.\n";
        return;
    }

    EVENT* newEvent = new EVENT{ title, 0, 0, 0, info, nullptr };
    formatDate(date, newEvent);

    addEventToFront(head, newEvent);
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
            std::cout << "\nDate: " << list->dateDay << "/" << list->dateMonth << "/" << list->dateYear << "\n";
            std::cout << "Info: " << list->info << "\n";
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
            std::cout << "Sorted events:\n";
            EVENT* curr = sortedHead;
            while (curr != nullptr) {
                std::cout << "- " << curr->title << " (" << curr->dateDay << "/" << curr->dateMonth << "/" << curr->dateYear << ")\n";
                curr = curr->next;
            }
            std::cout << "\nPress Enter to return...";
            std::cin.ignore();
            std::cin.get();
            return;
        }
    }
}

void displayEvents(EVENT* head, int& userId, sqlite3* db) {
    system("cls");
    showEventList(head);
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();

    int currentSelection = 0;
    while (true) {
        system("cls");
        showEventList(head);

        std::cout << "\nUse up/down arrows to navigate, Enter to select\n\n";
        std::cout << (currentSelection == 0 ? "> " : "  ") << "Display more info\n";
        std::cout << (currentSelection == 1 ? "> " : "  ") << "Sort events\n";
        std::cout << (currentSelection == 2 ? "> " : "  ") << "Edit event\n";
        std::cout << (currentSelection == 3 ? "> " : "  ") << "Delete event\n";
        std::cout << (currentSelection == 4 ? "> " : "  ") << "Go back to main menu\n";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
            case 72:
                if (currentSelection > 0)
                    currentSelection--;
                break;
            case 80:
                if (currentSelection < 4)
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
            std::string newTitle, newDate, newInfo;

            std::cout << "Enter new title (leave empty to keep current): ";
            std::getline(std::cin, newTitle);
            if (newTitle.empty()) newTitle = event->title;

            std::cout << "Enter new date (leave empty to keep current): ";
            std::getline(std::cin, newDate);
            if (newDate.empty()) {
                newDate = std::to_string(event->dateDay) + "/" + std::to_string(event->dateMonth) + "/" + std::to_string(event->dateYear);
            }

            std::cout << "Enter new info (leave empty to keep current): ";
            std::getline(std::cin, newInfo);
            if (newInfo.empty()) newInfo = event->info;

            if (auth.updateEvent(userId, event->title, newTitle, newDate, newInfo)) {
                event->title = newTitle;
                formatDate(newDate, event);
                event->info = newInfo;
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