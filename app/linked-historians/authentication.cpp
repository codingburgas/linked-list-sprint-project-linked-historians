#include "authentication.h"

Authentication::Authentication() : db(nullptr) {
    std::string dbPath = "../data/database.db";
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cout << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

Authentication::Authentication(sqlite3* db) : db(db) {
    std::string dbPath = "../data/database.db";
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cout << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

Authentication::~Authentication() {
    if (db) {
        sqlite3_close(db);
    }
}

bool Authentication::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool Authentication::executePreparedStatement(const std::string& query, sqlite3_stmt** stmt) {
    if (sqlite3_prepare_v2(db, query.c_str(), -1, stmt, nullptr) != SQLITE_OK) {
        std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return true;
}

bool Authentication::createTable() {
    std::string query = "CREATE TABLE IF NOT EXISTS accounts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL);";
    return executeQuery(query);
}

bool Authentication::createEventsTable() {
    std::string query = "CREATE TABLE IF NOT EXISTS events ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER NOT NULL, "
        "title TEXT NOT NULL, "
        "date TEXT NOT NULL, "
        "info TEXT NOT NULL, "
        "FOREIGN KEY(user_id) REFERENCES accounts(id));";
    return executeQuery(query);
}

bool Authentication::signUp(std::string& username, std::string& password, int& userId) {
    std::ostringstream query;
    query << "INSERT INTO accounts (username, password) VALUES ('"
        << username << "', '" << password << "');";

    if (!executeQuery(query.str())) {
        return false;
    }

    userId = static_cast<int>(sqlite3_last_insert_rowid(db));
    return true;
}

bool Authentication::logIn(std::string& username, std::string& password, int& userId) {
    std::ostringstream query;
    query << "SELECT id FROM accounts WHERE username='" << username
        << "' AND password='" << password << "';";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userId = sqlite3_column_int(stmt, 0);
        success = true;
    }

    sqlite3_finalize(stmt);
    return success;
}

bool Authentication::addEvent(int userId, const std::string& title, const std::string& date, const std::string& info) {
    std::ostringstream query;
    query << "INSERT INTO events (user_id, title, date, info) VALUES ("
        << userId << ", '" << title << "', '" << date << "', '" << info << "');";

    return executeQuery(query.str());
}

bool Authentication::fetchEvents(int userId, EVENT** head) {
    std::ostringstream query;
    query << "SELECT title, date, info FROM events WHERE user_id = " << userId << ";";

    sqlite3_stmt* stmt;
    if (!executePreparedStatement(query.str(), &stmt)) {
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        EVENT* newEvent = new EVENT();
        newEvent->title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string dateStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        formatDate(dateStr, newEvent);
        newEvent->info = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        newEvent->next = nullptr;

        if (*head == nullptr) {
            *head = newEvent;
        }
        else {
            EVENT* temp = *head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newEvent;
        }
    }

    sqlite3_finalize(stmt);
    return true;
}

bool Authentication::deleteEvent(int userId, const std::string& title) {
    std::ostringstream query;
    query << "DELETE FROM events WHERE user_id = " << userId << " AND title = '" << title << "';";

    return executeQuery(query.str());
}

bool Authentication::updateEvent(int userId, const std::string& oldTitle, const std::string& newTitle, const std::string& newDate, const std::string& newInfo) {
    std::ostringstream query;
    query << "UPDATE events SET title = '" << newTitle
        << "', date = '" << newDate
        << "', info = '" << newInfo
        << "' WHERE user_id = " << userId
        << " AND title = '" << oldTitle << "';";

    return executeQuery(query.str());
}

void formatDate(const std::string& dateStr, EVENT* event) {
    if (!event) return;

    std::stringstream ss(dateStr);
    char delimiter;
    int day, month, year;

    if (ss >> day >> delimiter >> month >> delimiter >> year) {
		event->dateDay = day;
		event->dateMonth = month;
		event->dateYear = year;
    }
    else {
        std::cout << "Invalid date format: " << dateStr << std::endl;
    }
}
