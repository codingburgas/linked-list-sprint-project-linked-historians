#include "utilities.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <conio.h>

namespace utilities {

    std::string chooseEventType() {
        const std::string options[] = { "War", "Revolution", "Battle", "Treaty", "Discovery", "Conference", "Festival", "Coronation", "Accident" };
        int count = 9;
        int selection = 0;
        while (true) {
            clearScreen();
            std::cout << "Choose Event Type:\n";
            for (int i = 0; i < count; i++) {
                std::cout << (selection == i ? "> " : "  ") << options[i] << "\n";
            }
            int key = _getch();
            if (key == 224) {
                key = _getch();
                switch (key) {
                case 72: if (selection > 0) selection--; break;
                case 80: if (selection < count - 1) selection++; break;
                }
            }
            else if (key == 13) {
                return options[selection];
            }
        }
    }

    bool getValidInput(const std::string& prompt, std::string& input) {
        std::cout << prompt << ": ";
        std::getline(std::cin, input);
        return !isEmpty(input);
    }

    bool getValidDateInput(const std::string& prompt, std::string& date) {
        while (true) {
            std::cout << prompt << ": ";
            std::getline(std::cin, date);
            if (date.empty()) {
                std::cout << "Date cannot be empty.\n";
                continue;
            }
            if (!isValidDateFormat(date)) {
                std::cout << "Invalid date format. Please enter date in DD/MM/YYYY format (1/1/1 - 7/4/2025).\n";
                continue;
            }
            return true;
        }
    }

    bool isEmpty(const std::string& str) {
        return str.empty();
    }

    bool isValidDateFormat(const std::string& date) {
        size_t firstSlash = date.find('/');
        if (firstSlash == std::string::npos)
            return false;
        size_t secondSlash = date.find('/', firstSlash + 1);
        if (secondSlash == std::string::npos)
            return false;

        std::string dayStr = date.substr(0, firstSlash);
        std::string monthStr = date.substr(firstSlash + 1, secondSlash - firstSlash - 1);
        std::string yearStr = date.substr(secondSlash + 1);

        if (dayStr.empty() || dayStr.size() > 2)
            return false;
        if (monthStr.empty() || monthStr.size() > 2)
            return false;
        if (yearStr.empty() || yearStr.size() > 4)
            return false;

        if (!std::all_of(dayStr.begin(), dayStr.end(), ::isdigit) ||
            !std::all_of(monthStr.begin(), monthStr.end(), ::isdigit) ||
            !std::all_of(yearStr.begin(), yearStr.end(), ::isdigit))
            return false;

        int day = std::stoi(dayStr);
        int month = std::stoi(monthStr);
        int year = std::stoi(yearStr);

        if (day <= 0 || month <= 0 || year < 0)
            return false;
        const int maxYear = 2025;
        const int maxMonth = 4;
        const int maxDay = 7;
        if (year > maxYear)
            return false;
        if (year == maxYear) {
            if (month > maxMonth)
                return false;
            if (month == maxMonth && day > maxDay)
                return false;
        }
        if (month < 1 || month > 12)
            return false;
        int daysInMonth;
        switch (month) {
        case 2: {
            bool isLeap = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
            daysInMonth = isLeap ? 29 : 28;
            break;
        }
        case 4: case 6: case 9: case 11:
            daysInMonth = 30;
            break;
        default:
            daysInMonth = 31;
        }
        if (day > daysInMonth)
            return false;

        return true;
    }

    bool isValidType(const std::string& type) {
        return type.empty() || type == "War" || type == "Revolution" || type == "Battle" ||
            type == "Treaty" || type == "Discovery" || type == "Conference" ||
            type == "Festival" || type == "Coronation" || type == "Accident";
    }

    bool containsString(const std::string& str, const std::string& substr) {
        std::string strLower = str, substrLower = substr;
        std::transform(strLower.begin(), strLower.end(), strLower.begin(), ::tolower);
        std::transform(substrLower.begin(), substrLower.end(), substrLower.begin(), ::tolower);
        return strLower.find(substrLower) != std::string::npos;
    }

    bool matchDate(const std::string& inputDate, const EVENT* event) {
        EVENT tempEvent{ "", 0, 0, 0, "", nullptr, "" };
        formatDate(inputDate, &tempEvent);
        return (tempEvent.dateDay == event->dateDay &&
            tempEvent.dateMonth == event->dateMonth &&
            tempEvent.dateYear == event->dateYear);
    }

    void clearScreen() {
        system("cls");
    }

    void waitForEnter() {
        std::cout << "\nPress ENTER to continue...";
        std::cin.ignore();
        std::cin.get();
    }

    void displaySortMenu(int currentSelection) {
        const std::string options[5] = {
            "Title (Ascending)",
            "Title (Descending)",
            "Date (Ascending)",
            "Date (Descending)",
            "Go back"
        };
        for (int i = 0; i < 5; i++) {
            std::cout << (i == currentSelection ? "> " : "  ") << options[i] << "\n";
        }
    }

    void displayEventsMenu(int currentSelection) {
        const std::string options[9] = {
            "Display more info",
            "Sort events",
            "Edit event",
            "Delete event",
            "Set type filter",
            "Set name filter",
            "Set date filter",
            "Clear all filters",
            "Go back to main menu"
        };
        for (int i = 0; i < 9; i++) {
            std::cout << (i == currentSelection ? "> " : "  ") << options[i] << "\n";
        }
    }

    void showFilteredEventList(EVENT* head, const std::string& typeFilter,
        const std::string& nameFilter, const std::string& dateFilter) {
        std::cout << "Available events";
        if (!typeFilter.empty()) std::cout << " (filtered by type: " << typeFilter << ")";
        if (!nameFilter.empty()) std::cout << " (filtered by name: " << nameFilter << ")";
        if (!dateFilter.empty()) std::cout << " (filtered by date: " << dateFilter << ")";
        std::cout << ":\n";
        bool anyDisplayed = false;
        if (head == nullptr) {
            std::cout << "No historical events recorded.\n";
        }
        else {
            EVENT* list = head;
            while (list != nullptr) {
                if (eventMatchesFilters(list, typeFilter, nameFilter, dateFilter)) {
                    std::cout << "- " << list->title << "\n";
                    anyDisplayed = true;
                }
                list = list->next;
            }
            if (!anyDisplayed) {
                std::cout << "No events match the current filters.\n";
            }
        }
    }

    bool eventMatchesFilters(const EVENT* event, const std::string& typeFilter,
        const std::string& nameFilter, const std::string& dateFilter) {
        bool matchesType = typeFilter.empty() || event->type == typeFilter;
        bool matchesName = nameFilter.empty() || containsString(event->title, nameFilter);
        bool matchesDate = dateFilter.empty() || matchDate(dateFilter, event);
        return matchesType && matchesName && matchesDate;
    }

}
