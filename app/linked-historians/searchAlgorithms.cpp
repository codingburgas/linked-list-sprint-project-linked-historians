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
