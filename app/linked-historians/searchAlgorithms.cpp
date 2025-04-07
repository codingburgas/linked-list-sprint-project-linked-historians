#include "event.h"
#include "searchAlgorithms.h"

bool containsSubstring(const std::string& str, const std::string& substring) {
    std::string strLower = str;
    std::string substringLower = substring;

    // Convert both strings to lowercase to make the search case-insensitive
    std::transform(strLower.begin(), strLower.end(), strLower.begin(), ::tolower);
    std::transform(substringLower.begin(), substringLower.end(), substringLower.begin(), ::tolower);

    return strLower.find(substringLower) != std::string::npos;
}

/*void searchInEvent(EVENT* head, const std::string& searchKeyword) {
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

}*/