#include "authentication.h"

Authentication::Authentication() {
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

bool Authentication::createTable() {
    std::string query = "CREATE TABLE IF NOT EXISTS accounts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL);";
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
