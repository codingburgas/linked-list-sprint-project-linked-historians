#include "pch.h"
#include "utilities.h"

namespace utilities {

    bool getValidInput(const std::string& prompt, std::string& input) {
        std::cout << prompt << ": ";
        std::getline(std::cin, input);
        return !isEmpty(input);
    }

    bool isEmpty(const std::string& str) {
        return str.empty();
    }

    bool isValidDateFormat(const std::string& date) {
        if (date.empty()) return true;
        try {
            EVENT tempEvent{ "", 0, 0, 0, "", nullptr, "" };
            formatDate(date, &tempEvent);
            return true;
        }
        catch (...) {
            return false;
        }
    }

    bool isValidType(const std::string& type) {
        return type.empty() || type == "War" || type == "Revolution";
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
        std::cout << "\nPress Enter to continue...";
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
        std::string options[9] = {
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