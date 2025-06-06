#pragma once

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

struct  EVENT {
    std::string title;
    int dateDay;
    int dateMonth;
    int dateYear;
    std::string info;
    EVENT* next;
    std::string type;
};

class Authentication {
public:
    Authentication();
    Authentication(sqlite3* db);
    ~Authentication();
    bool executeQuery(const std::string& query);
    bool createTable();
    bool createEventsTable();
    bool signUp(std::string& username, std::string& password, int& userId);
    bool logIn(std::string& username, std::string& password, int& userId);

    bool addEvent(int userId, const std::string& title, const std::string& date,
        const std::string& info, const std::string& type);
    bool fetchEvents(int userId, EVENT** head);
    bool deleteEvent(int userId, const std::string& title);
    bool updateEvent(int userId, const std::string& oldTitle,
        const std::string& newTitle, const std::string& newDate,
        const std::string& newInfo, const std::string& newType);

    sqlite3* getDb() const { return db; }

private:
    sqlite3* db;
    bool executePreparedStatement(const std::string& query, sqlite3_stmt** stmt);
};

void formatDate(const std::string& dateStr, EVENT* event);