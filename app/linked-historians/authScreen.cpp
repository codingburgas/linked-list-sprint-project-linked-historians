#include "authScreen.h"
#include "menu.h"

// Calls the functions to initialize the database
AuthScreen::AuthScreen() {
    if (!auth.createTable() || !auth.createEventsTable()) {
        std::cerr << "Failed to initialize the database.\n";
    }
}

// Function to display the name of the team  with colored letters
void changeColor()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string text = " LINKED HISTORIANS ";
    int consoleWidth = 80;
    int textWidth = text.length() + 19;
    int padding = (consoleWidth - textWidth) / 2;


    SetConsoleTextAttribute(h, 15);
    std::cout << std::setw(padding) << "";
    for (int i = 0; i < textWidth; i++)
    {
        std::cout << "-";
    }
    std::cout << "\n";


    std::cout << std::setw(padding) << "";
    SetConsoleTextAttribute(h, 11);
    std::cout << std::setw(29) << text;
    SetConsoleTextAttribute(h, 15);
    std::cout << std::setw(padding) << "" << std::endl;


    std::cout << std::setw(padding) << " ";
    for (int i = 0; i < textWidth; i++) {
        std::cout << "-";
    }
    std::cout << "\n";


    SetConsoleTextAttribute(h, 14);
    std::cout << "\n";
}

// Function to display the options in the authScreen
void AuthScreen::displayLoginOptions(int currentSelection) {
    std::cout << "Use UP/DOWN arrows to navigate, ENTER to select\n\n";

    std::cout << (currentSelection == 0 ? "> " : "  ") << "1. Sign Up\n";
    std::cout << (currentSelection == 1 ? "> " : "  ") << "2. Log In\n";
    std::cout << (currentSelection == 2 ? "> " : "  ") << "3. Exit\n";
}

// Function to allow users to navigate and select options
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

// Function to handle the sign-up process
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
        }
        else if (ch != '\b') {
            password.push_back(ch);
            std::cout << '*';
        }
    }
    std::cout << '\n';

    if (auth.signUp(username, password, userId)) {
        displayMenu(userId, auth);
    }
    else {
        std::cout << "Sign-up failed. Username is already taken.\n";
    }
}

// Function to handle the login process
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
        }
        else if (ch != '\b') {
            password.push_back(ch);
            std::cout << '*';
        }
    }
    std::cout << '\n';

    if (auth.logIn(username, password, userId)) {
        displayMenu(userId, auth);
    }
    else {
        std::cout << "Login failed. Invalid username or password.\n";
    }
}