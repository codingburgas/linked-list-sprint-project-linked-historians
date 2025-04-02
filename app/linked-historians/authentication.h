#pragma once

#include "pch.h"
#include "event.h"

struct EVENT;

class Authentication {
public:
    Authentication();
    Authentication(sqlite3* db);
    ~Authentication();

    bool createTable();
    bool createEventsTable();
    bool signUp(std::string& username, std::string& password, int& userId);
    bool logIn(std::string& username, std::string& password, int& userId);

    bool addEvent(int userId, const std::string& title, const std::string& date, const std::string& info);
    bool fetchEvents(int userId, EVENT** head);
    bool deleteEvent(int userId, const std::string& title);
    bool updateEvent(int userId, const std::string& oldTitle, const std::string& newTitle, const std::string& newDate, const std::string& newInfo);

    sqlite3* getDb() const { return db; }

private:
    sqlite3* db;
    bool executeQuery(const std::string& query);
    bool executePreparedStatement(const std::string& query, sqlite3_stmt** stmt);
};
