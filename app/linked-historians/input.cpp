#include "input.h"
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <windows.h>
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <algorithm>
#include <winsqlite/winsqlite3.h>
#pragma comment(lib, "winsqlite3.lib")

#include <sstream>
#include <cctype>

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

