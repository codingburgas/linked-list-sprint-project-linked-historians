#pragma once
#include "pch.h"
#include "authentication.h"
#include "menu.h"

class AuthScreen {
public:
    AuthScreen();
    void displayAuthMenu();
    void displayLoginOptions(int currentSelection);

private:
    Authentication auth;
    void handleSignUp();
    void handleLogIn();
    
};
