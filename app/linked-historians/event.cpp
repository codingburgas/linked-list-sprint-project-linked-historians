#include "event.h"
#include "searchAlgorithms.h"

void addEvent(EVENT** head, int& userId, sqlite3* db) {
    static int count = 0;
    if (count == 0) {
        std::cin.ignore();
        count++;
    }
    std::string title, date, info;

    std::cout << "Enter Title: ";
    std::getline(std::cin, title);
    std::cout << "Enter Date: ";
    std::getline(std::cin, date);
    std::cout << "Enter Info: ";
    std::getline(std::cin, info);

    std::ostringstream query;
    query << "INSERT INTO events (user_id, title, date, info) VALUES ("
          << userId << ", '" << title << "', '" << date << "', '" << info << "');";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error inserting event: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return;
    }

    EVENT* newEvent = new EVENT{ title, date, info, nullptr };
    if (*head == nullptr) {
        *head = newEvent;
    } else {
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
			std::cout << "Available events: \n";
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
				}
				list = list->next;
				std::cout << "\n";
			}
	}

	void searchInEvent(EVENT* head, const std::string& searchKeyword)
	{
		EVENT* list = head;
		bool keywordFound = false;


		std::cout << "Available events: \n";
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