#include "authScreen.h"
AuthScreen::AuthScreen() {
    if (!auth.createTable() || !auth.createEventsTable()) {
        std::cerr << "Failed to initialize the database.\n";
    }
}

void AuthScreen::displayAuthMenu() {
    char choice = ' ';
    do {
        std::cout << "--- Authentication Menu ---\n";
        std::cout << "1. Sign Up\n";
        std::cout << "2. Log In\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        choice = _getch();

        switch (choice) {
        case '1':
            handleSignUp();
            break;
        case '2':
            handleLogIn();
            break;
        case '3':
            break;
        default:
            break;
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