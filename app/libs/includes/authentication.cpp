#include "authentication.h"
#include "sha256.h"

// Initializes the object and opens the SQLite database
Authentication::Authentication() : db(nullptr) {
    std::string dbPath = "../data/database.db";
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cout << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

// Initializes the object with an SQLite database connection
Authentication::Authentication(sqlite3* db) : db(db) {
    std::string dbPath = "../data/database.db";
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cout << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

//Closes the SQLite database connection 
Authentication::~Authentication() {
    if (db) {
        sqlite3_close(db);
    }
}

// Executes an SQL query
bool Authentication::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// Prepares an SQL statement 
bool Authentication::executePreparedStatement(const std::string& query, sqlite3_stmt** stmt) {
    if (sqlite3_prepare_v2(db, query.c_str(), -1, stmt, nullptr) != SQLITE_OK) {
        std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return true;
}

// Creates the accounts table in the database if it does not already exist
bool Authentication::createTable() {
    std::string query = "CREATE TABLE IF NOT EXISTS accounts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL);";
    return executeQuery(query);
}

// Creates the events table in the database if it does not already exist
bool Authentication::createEventsTable() {
    std::string query = "CREATE TABLE IF NOT EXISTS events ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER NOT NULL, "
        "title TEXT NOT NULL, "
        "date TEXT NOT NULL, "
        "info TEXT NOT NULL, "
        "type TEXT NOT NULL, "
        "FOREIGN KEY(user_id) REFERENCES accounts(id));";
    return executeQuery(query);
}

// Registers a new user by inserting the given username and password into the accounts table.
bool Authentication::signUp(std::string& username, std::string& password, int& userId) {
    std::string hashedPassword = sha256(password);
    std::ostringstream query;
    query << "INSERT INTO accounts (username, password) VALUES ('"
        << username << "', '" << hashedPassword << "');";

    if (!executeQuery(query.str())) {
        return false;
    }

    userId = static_cast<int>(sqlite3_last_insert_rowid(db));
    return true;
}

// Logs in a user by verifying their username and hashed password against the "accounts" table.
bool Authentication::logIn(std::string& username, std::string& password, int& userId) {
    std::string hashedPassword = sha256(password);
    std::ostringstream query;
    query << "SELECT id FROM accounts WHERE username='" << username
        << "' AND password='" << hashedPassword << "';";

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

// Adds a new event to the events table for the user with given id
bool Authentication::addEvent(int userId, const std::string& title, const std::string& date,
    const std::string& info, const std::string& type) {
    std::ostringstream query;
    query << "INSERT INTO events (user_id, title, date, info, type) VALUES ("
        << userId << ", '" << title << "', '" << date << "', '" << info << "', '" << type << "');";

    return executeQuery(query.str());
}

// Fetches all events for a user from the events table and stores them in the linked list.
bool Authentication::fetchEvents(int userId, EVENT** head) {
    std::ostringstream query;
    query << "SELECT title, date, info, type FROM events WHERE user_id = " << userId << ";";

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
        newEvent->type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
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

// Deletes an event with a given name from the events table for a specific userId
bool Authentication::deleteEvent(int userId, const std::string& title) {
    std::ostringstream query;
    query << "DELETE FROM events WHERE user_id = " << userId << " AND title = '" << title << "';";

    return executeQuery(query.str());
}

// Updates an existing event for a specific user in the "events" table.
bool Authentication::updateEvent(int userId, const std::string& oldTitle,
    const std::string& newTitle, const std::string& newDate,
    const std::string& newInfo, const std::string& newType) {
    std::ostringstream query;
    query << "UPDATE events SET title = '" << newTitle
        << "', date = '" << newDate
        << "', info = '" << newInfo
        << "', type = '" << newType
        << "' WHERE user_id = " << userId
        << " AND title = '" << oldTitle << "';";

    return executeQuery(query.str());
}

// Receives a string in the format "DD-MM-YYYY" and stores the values in an EVENT object
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
