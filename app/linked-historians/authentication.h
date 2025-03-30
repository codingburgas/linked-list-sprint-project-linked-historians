#pragma

#include "pch.h"

class Authentication {
public:
    Authentication();
    ~Authentication();

    bool createTable();
    bool signUp(std::string& username, std::string& password, int& userId);
    bool logIn(std::string& username, std::string& password, int& userId);

private:
    sqlite3* db;
    bool executeQuery(const std::string& query);
};


