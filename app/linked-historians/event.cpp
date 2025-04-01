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

    std::ostringstream query;
    query << "INSERT INTO events (user_id, title, date, info) VALUES ("
        << userId << ", '" << title << "', '" << date << "', '" << info << "');";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cout << "Error inserting event: " << errMsg << std::endl;
        sqlite3_free(errMsg);
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
    EVENT* sortedHead = sortEventsByTitle(head);
    system("cls");
    std::cout << "Sorted events:\n";
    EVENT* curr = sortedHead;
    while (curr != nullptr) {
        std::cout << "- " << curr->title << "\n";
        curr = curr->next;
    }
    std::cout << "\nPress Enter to return to the submenu...";
    std::cin.ignore();
    std::cin.get();
}

void displayEvents(EVENT* head)
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
        std::cout << (currentSelection == 2 ? "> " : "  ") << "Go back to main menu\n";

        int key = _getch();
        if (key == 224) {  
            key = _getch();
            switch (key) {
            case 72: 
                if (currentSelection > 0)
                    currentSelection--;
                break;
            case 80: 
                if (currentSelection < 2)
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
                break;
            }
        }
    }
}

void searchInEvent(EVENT* head, const std::string& searchKeyword)
{
    EVENT* list = head;
    bool keywordFound = false;

    std::cout << "Available events:\n";
    if (list == nullptr) {
        std::cout << "No historical events recorded.\n";
        return;
    }

    while (list != nullptr)
    {
        std::cout << "- " << list->title << "\n";
        list = list->next;
    }

    list = head;
    std::cout << "\nEnter the name of the event to view details: ";
    std::string eventTitle;
    std::getline(std::cin, eventTitle);

    while (list != nullptr)
    {
        if (list->title == eventTitle)
        {
            std::cout << "\nEnter a keyword to search in the info: ";
            std::string keyword;
            std::getline(std::cin, keyword);

            if (containsSubstring(list->info, keyword)) {
                std::cout << "Keyword found in the event info!\n";
                std::cout << "Info: " << list->info << "\n";
                keywordFound = true;
                std::cout << "\n";
            }

            break;
        }
        list = list->next;

        if (!keywordFound) {
            std::cout << "Keyword not found in the event's info.\n";
        }
    }
}

void fetchEvents(EVENT** head, int userId, sqlite3* db) {
    std::ostringstream query;
    query << "SELECT title, date, info FROM events WHERE user_id = " << userId << ";";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        EVENT* newEvent = new EVENT();
        newEvent->title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        newEvent->date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        newEvent->info = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        newEvent->next = *head;
        *head = newEvent;
    }
    
    sqlite3_finalize(stmt);
}