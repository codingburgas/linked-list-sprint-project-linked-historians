#pragma once
#include "authentication.h"
#include <string>

namespace utilities {
    std::string chooseEventType();
    bool getValidInput(const std::string& prompt, std::string& input);
    bool getValidDateInput(const std::string& prompt, std::string& date);
    bool isEmpty(const std::string& str);
    bool isValidDateFormat(const std::string& date);
    bool isValidType(const std::string& type);
    bool containsString(const std::string& str, const std::string& substr);
    bool matchDate(const std::string& inputDate, const EVENT* event);
    void clearScreen();
    void waitForEnter();
    void displayEventsMenu(int currentSelection);
    void displaySortMenu(int currentSelection);
    void showFilteredEventList(EVENT* head, const std::string& typeFilter,
        const std::string& nameFilter, const std::string& dateFilter);
    bool eventMatchesFilters(const EVENT* event, const std::string& typeFilter,
        const std::string& nameFilter, const std::string& dateFilter);
}
