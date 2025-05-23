#include "event.h"
#include "searchAlgorithms.h"
#include "sortingAlgorithms.h"
#include "utilities.h"
#include "utilities.cpp"
#include <iostream>
#include <string>
#include <conio.h>

// Add an event to the start of the linked list
void addEventToFront(EVENT** head, EVENT* newEvent) {
    newEvent->next = *head;
    *head = newEvent;
}

// Function to add a new event
void addEvent(EVENT** head, int& userId, sqlite3* db) {
    static int count = 0;
    if (count == 0) {
        std::cin.ignore();
        count++;
    }
    std::string title, date, info;
    while (!utilities::getValidInput("Enter Title", title))
        std::cout << "Error: Title cannot be empty.\n";
    utilities::getValidDateInput("Enter Date (DD/MM/YYYY)", date);
    while (!utilities::getValidInput("Enter Info", info))
        std::cout << "Error: Info cannot be empty.\n";
    std::string type = utilities::chooseEventType();
    Authentication auth(db);
    bool success = auth.addEvent(userId, title, date, info, type);
    if (!success) {
        std::cout << "Error inserting event into database.\n";
        return;
    }
    EVENT* newEventObj = new EVENT{ title, 0, 0, 0, info, nullptr, type };
    formatDate(date, newEventObj);
    addEventToFront(head, newEventObj);
}

// Function to display the full information about a specific event
void displayEventInfo(EVENT* head, const std::string& typeFilter, const std::string& nameFilter, const std::string& dateFilter) {
    utilities::clearScreen();
    std::string searchTitle;
    std::cout << "Enter the name of the event to view details: ";
    std::getline(std::cin, searchTitle);
    EVENT* list = head;
    bool found = false;
    while (list != nullptr) {
        if (utilities::eventMatchesFilters(list, typeFilter, nameFilter, dateFilter) && list->title == searchTitle) {
            std::cout << "\nDate: " << list->dateDay << "/" << list->dateMonth << "/" << list->dateYear << "\n";
            std::cout << "Info: " << list->info << "\n";
            std::cout << "Type: " << list->type << "\n";
            found = true;
            break;
        }
        list = list->next;
    }
    if (!found)
        std::cout << "\nEvent not found.\n";
    utilities::waitForEnter();
}

// Function to display events sorted by various criteria
void displaySortedEvents(EVENT*& head, const std::string& typeFilter, const std::string& nameFilter, const std::string& dateFilter) {
    utilities::clearScreen();
    int sortSelection = 0;
    while (true) {
        utilities::clearScreen();
        std::cout << "Choose sorting order:\n";
        utilities::displaySortMenu(sortSelection);
        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
            case 72: if (sortSelection > 0) sortSelection--; break;
            case 80: if (sortSelection < 4) sortSelection++; break;
            }
        }
        else if (key == 13) {
            if (sortSelection == 4) return;
            switch (sortSelection) {
            case 0: head = sortEventsByTitle(head); break;
            case 1: head = sortEventsByTitleDescending(head); break;
            case 2: head = sortEventsByDate(head); break;
            case 3: head = sortEventsByDateDescending(head); break;
            }
            utilities::clearScreen();
            utilities::showFilteredEventList(head, typeFilter, nameFilter, dateFilter);
            utilities::waitForEnter();
        }

        std::cout << "Sorted events";
        if (!typeFilter.empty()) std::cout << " (filtered by type: " << typeFilter << ")";
        if (!nameFilter.empty()) std::cout << " (filtered by name: " << nameFilter << ")";
        if (!dateFilter.empty()) std::cout << " (filtered by date: " << dateFilter << ")";
        std::cout << ":\n";
        EVENT* curr = head;
        bool anyDisplayed = false;
        while (curr != nullptr) {
            if (utilities::eventMatchesFilters(curr, typeFilter, nameFilter, dateFilter)) {
                std::cout << "- " << curr->title << " (" << curr->dateDay << "/" << curr->dateMonth << "/" << curr->dateYear << ")\n";
                anyDisplayed = true;
            }
            curr = curr->next;
        }
        if (!anyDisplayed)
            std::cout << "No events match the current filters.\n";
        utilities::waitForEnter();
        return;
    }
}

// Display all events with the menu for filtering and navigation options
void displayEvents(EVENT* head, int& userId, sqlite3* db) {
    std::string currentTypeFilter = "";
    std::string currentNameFilter = "";
    std::string currentDateFilter = "";
    std::string searchKeyword = "";
    utilities::clearScreen();
    utilities::showFilteredEventList(head, currentTypeFilter, currentNameFilter, currentDateFilter);
    utilities::waitForEnter();
    int currentSelection = 0;
    while (true) {
        utilities::clearScreen();
        utilities::showFilteredEventList(head, currentTypeFilter, currentNameFilter, currentDateFilter);
        std::cout << "\nCurrent filters:\n";
        std::cout << "Type: " << (currentTypeFilter.empty() ? "None" : currentTypeFilter) << "\n";
        std::cout << "Name: " << (currentNameFilter.empty() ? "None" : currentNameFilter) << "\n";
        std::cout << "Date: " << (currentDateFilter.empty() ? "None" : currentDateFilter) << "\n";
        std::cout << "\nUse UP/DOWN arrows to navigate, Enter to select\n\n";
        utilities::displayEventsMenu(currentSelection);
        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
            case 72: if (currentSelection > 0) currentSelection--; break;
            case 80: if (currentSelection < 9) currentSelection++; break;
            }
        }
        else if (key == 13) {
            if (currentSelection == 0) {
                displayEventInfo(head, currentTypeFilter, currentNameFilter, currentDateFilter);
            }
            else if (currentSelection == 1) {
                displaySortedEvents(head, currentTypeFilter, currentNameFilter, currentDateFilter);
            }
            else if (currentSelection == 2) {
                editEvent(head, userId, db);
                utilities::waitForEnter();
            }
            else if (currentSelection == 3) {
                std::string title;
                std::cout << "\nEnter the title of the event to delete: ";
                std::getline(std::cin, title);
                deleteEvent(&head, title, userId, db);
                utilities::waitForEnter();
            }
            else if (currentSelection == 4) {
                std::string type;
                std::cout << "\nEnter filter type (War, Revolution, Battle, Treaty, Discovery, Conference, Festival, Coronation, Accident or leave empty for no filter): ";
                std::getline(std::cin, type);
                if (type.empty() || utilities::isValidType(type)) {
                    currentTypeFilter = type;
                }
                else {
                    std::cout << "Invalid type. Please enter a valid type or leave empty.\n";
                    utilities::waitForEnter();
                    continue;
                }
            }
            else if (currentSelection == 5) {
                std::string name;
                std::cout << "\nEnter name filter (string to search in titles, leave empty for no filter): ";
                std::getline(std::cin, name);
                currentNameFilter = name;
            }
            else if (currentSelection == 6) {
                std::string date;
                std::cout << "\nEnter date filter (DD/MM/YYYY, leave empty for no filter): ";
                std::getline(std::cin, date);
                if (date.empty() || utilities::isValidDateFormat(date)) {
                    currentDateFilter = date;
                }
                else {
                    std::cout << "Invalid date format. Please use DD/MM/YYYY.\n";
                    utilities::waitForEnter();
                    continue;
                }
            }
            else if (currentSelection == 7) {
                currentTypeFilter = "";
                currentNameFilter = "";
                currentDateFilter = "";
            }
            else if (currentSelection == 8) {
                std::cout << "\nEnter a word to search for: ";
                std::getline(std::cin, searchKeyword);
                searchInEvent(head, searchKeyword);
                utilities::waitForEnter();
            }
            else if (currentSelection == 9) {
                break;
            }
        }
    }
}

// Function to delete an event
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
    if (temp == nullptr) return;
    prev->next = temp->next;
    delete temp;
    auth.fetchEvents(userId, head);
}

// Function to edit an existing event by its title
void editEvent(EVENT* head, int userId, sqlite3* db) {
    std::cout << "Enter the title of the event you want to edit: ";
    std::string title;
    std::getline(std::cin, title);
    Authentication auth(db);
    EVENT* event = head;
    while (event != nullptr) {
        if (event->title == title) {
            std::string newTitle, newDate, newInfo, newType;
            if (!utilities::getValidInput("Enter new title (leave empty to keep current)", newTitle) || newTitle.empty()) {
                newTitle = event->title;
            }
            if (!utilities::getValidInput("Enter new date (leave empty to keep current)", newDate) || newDate.empty()) {
                newDate = std::to_string(event->dateDay) + "/" + std::to_string(event->dateMonth) + "/" + std::to_string(event->dateYear);
            }
            if (!utilities::getValidInput("Enter new info (leave empty to keep current)", newInfo) || newInfo.empty()) {
                newInfo = event->info;
            }
            std::cout << "Press ENTER to keep current type (" << event->type << ") or any other key to change it.\n";
            int key = _getch();
            if (key == 13) {
                newType = event->type;
            }
            else {
                newType = utilities::chooseEventType();
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

// Function to search for a keyword in an event's information
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
    std::string eventTitle;
    std::getline(std::cin, eventTitle);
    while (list != nullptr) {
        if (list->title == eventTitle) {
            eventFound = true;
            std::cout << "\nEnter a keyword to search in the info: ";
            std::string keyword;
            std::getline(std::cin, keyword);
            if (utilities::containsString(list->info, keyword)) {
                std::cout << "Keyword found in the event info!\n";
                std::cout << "Info: " << list->info << "\n";
                keywordFound = true;
            }
            else {
                std::cout << "Keyword not found in the event's info.\n";
            }
            break;
        }
        list = list->next;
    }
    if (!eventFound) {
        std::cout << "Event not found.\n";
    }
    std::cin.get();
}
