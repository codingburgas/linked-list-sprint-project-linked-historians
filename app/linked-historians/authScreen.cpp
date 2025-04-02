#include "authScreen.h"
#include "menu.h"
AuthScreen::AuthScreen() {
    if (!auth.createTable() || !auth.createEventsTable()) {
        std::cerr << "Failed to initialize the database.\n";
    }
}

void changeColor()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 11);

}

 void AuthScreen::displayLoginOptions(int currentSelection) {
    system("cls");
    std::cout << "Use UP/DOWN arrows to navigate, ENTER to select\n\n";

    std::cout << (currentSelection == 0 ? "> " : "  ") << "1. Sign Up\n";
    std::cout << (currentSelection == 1 ? "> " : "  ") << "2. Log In\n";
    std::cout << (currentSelection == 2 ? "> " : "  ") << "3. Exit\n";
}



void AuthScreen::displayAuthMenu() {
    int choice;
    int currentSelection = 0;

    do {
        system("cls");
        changeColor();
        displayLoginOptions(currentSelection); 
        choice = _getch();

        if (choice == 224) { 
            choice = _getch();
            switch (choice) {
            case 72: 
                if (currentSelection > 0) currentSelection--;
                break;
            case 80: 
                if (currentSelection < 2) currentSelection++;
                break;
            }
        }
        else if (choice == 13) {
            switch (currentSelection) {
            case 0: handleSignUp(); break;
            case 1: handleLogIn(); break;
            case 2: return; 
            }
        }
    } while (choice != '3');
}

void AuthScreen::handleSignUp() {
    std::string username, password;
    int userId;

    std::cout << "\nEnter username: ";
    std::cin >> username;

    std::cout << "Enter password: ";
    char ch;
    while ((ch = _getch()) != '\r') { 
        if (ch == '\b' && !password.empty()) { 
            password.pop_back();
            std::cout << "\b \b"; 
        } else if (ch != '\b') {
            password.push_back(ch);
            std::cout << '*'; 
        }
    }
    std::cout << '\n';

    if (auth.signUp(username, password, userId)) {
        displayMenu(userId,auth);
    } else {
        std::cout << "Sign-up failed. Username is already taken.\n";
    }
}

void AuthScreen::handleLogIn() {
    std::string username, password;
    int userId;

    std::cout << "\nEnter username: ";
    std::cin >> username;

    std::cout << "Enter password: ";
    char ch;
    while ((ch = _getch()) != '\r') { 
        if (ch == '\b' && !password.empty()) { 
            password.pop_back();
            std::cout << "\b \b"; 
        } else if (ch != '\b') {
            password.push_back(ch);
            std::cout << '*'; 
        }
    }
    std::cout << '\n';

    if (auth.logIn(username, password, userId)) {
        displayMenu(userId, auth);
    } else {
        std::cout << "Login failed. Invalid username or password.\n";
    }
}