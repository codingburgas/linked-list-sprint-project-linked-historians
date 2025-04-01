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
 
void displayEvents(EVENT* head)
{
    EVENT* list = head;
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

    std::cout << "\nEnter the name of the event to view details: ";
    std::string searchTitle;
    std::getline(std::cin, searchTitle);
    list = head;
    while (list != nullptr)
    {
        if (list->title == searchTitle)
        {
            std::cout << "Info: " << list->info << "\n";
            break;
        }
        list = list->next;
    }

    char choice;
    std::cout << "\nDo you want to display events sorted by title? (Y/N): ";
    std::cin >> choice;
    std::cin.ignore(); 

    if (choice == 'Y' || choice == 'y')
    {
        EVENT* sortedHead = sortEventsByTitle(head);
        std::cout << "\nSorted events:\n";
        EVENT* curr = sortedHead;
        while (curr != nullptr)
        {
            std::cout << "- " << curr->title << "\n";
            curr = curr->next;
        }
        
        std::cout << "\nEnter the name of the event to view details: ";
        std::string sortedSearchTitle;
        std::getline(std::cin, sortedSearchTitle);
        curr = sortedHead;
        while (curr != nullptr)
        {
            if (curr->title == sortedSearchTitle)
            {
                std::cout << "Info: " << curr->info << "\n";
                break;
            }
            curr = curr->next;
        }
    }
    std::cout << "\nPress Enter to return to the main menu...";
    std::cin.ignore();
    std::cin.get();
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