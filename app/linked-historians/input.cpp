#include "input.h"

bool getValidInput(const std::string& prompt, std::string& input) {
    std::cout << prompt << " (Press Enter twice to finish):\n";
    input.clear();
    std::string line;
    bool lastLineEmpty = false;

    while (true) {
        std::getline(std::cin, line);
        if (line.empty()) {
            if (lastLineEmpty) break;
            lastLineEmpty = true;
        }
        else {
            lastLineEmpty = false;
            if (!input.empty()) input += "\n";
            input += line;
        }
    }

    return !input.empty();
}