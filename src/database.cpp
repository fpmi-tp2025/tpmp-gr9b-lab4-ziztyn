#include "database.h"
#include <stdexcept>

Database::Database(const std::string& dbPath) {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_close(db);
        throw std::runtime_error("Не удалось открыть базу данных: " + errorMsg);
    }
}

Database::~Database() {
    sqlite3_close(db);
}

std::vector<std::vector<std::string>> Database::executeQuery(const std::string& query) {
    std::vector<std::vector<std::string>> result;
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Ошибка при подготовке SQL-запроса: " + errorMsg);
    }
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::vector<std::string> row;
        for (int i = 0; i < sqlite3_column_count(stmt); ++i) {
            const unsigned char* text = sqlite3_column_text(stmt, i);
            if (text) {
                row.push_back(reinterpret_cast<const char*>(text));
            } else {
                row.push_back("");
            }
        }
        result.push_back(row);
    }
    
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Ошибка при выполнении SQL-запроса: " + errorMsg);
    }
    
    sqlite3_finalize(stmt);
    return result;
}

int Database::getLastInsertRowId() const {
    return sqlite3_last_insert_rowid(db);
}