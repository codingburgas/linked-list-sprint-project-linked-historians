#pragma once
#include "pch.h"
#include "authentication.h"
#include "menu.h"

class AuthScreen {
public:
    AuthScreen();
    void displayAuthMenu();

private:
    Authentication auth;
    void handleSignUp();
    void handleLogIn();
};
