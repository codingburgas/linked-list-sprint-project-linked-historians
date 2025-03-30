#pragma once

#include "pch.h"

class Authentication {
public:
    Authentication(); 
    Authentication(sqlite3* db);
    ~Authentication();

    bool createTable();
    bool createEventsTable();
    bool signUp(std::string& username, std::string& password, int& userId);
    bool logIn(std::string& username, std::string& password, int& userId);
    sqlite3* getDb() const { return db; }
    
private:
    sqlite3* db;
    bool executeQuery(const std::string& query);
};
