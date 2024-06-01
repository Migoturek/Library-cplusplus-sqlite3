#include <iostream>
#include "database.hpp"


Database::Database(const std::string& db_name) {
    if (sqlite3_open(db_name.c_str(), &db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
    else {
        const char* create_table_sql = R"(
            CREATE TABLE IF NOT EXISTS books (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                title TEXT NOT NULL,
                author TEXT NOT NULL
            );
        )";
        char* err_msg = nullptr;
        if (sqlite3_exec(db, create_table_sql, 0, 0, &err_msg) != SQLITE_OK) {
            std::cerr << "SQL error: " << err_msg << std::endl;
            sqlite3_free(err_msg);
        }
    }
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

bool Database::addBook(const std::string& title, const std::string& author) {
    const char* insert_sql = "INSERT INTO books (title, author) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, author.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool Database::deleteBook(int id) {
    const char* delete_sql = "DELETE FROM books WHERE id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, delete_sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

std::vector<std::pair<int, std::string>> Database::searchBookByTitle(const std::string& title) {
    const char* search_sql = "SELECT id, title FROM books WHERE title LIKE ?;";
    sqlite3_stmt* stmt;
    std::vector<std::pair<int, std::string>> results;
    if (sqlite3_prepare_v2(db, search_sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return results;
    }
    std::string title_pattern = "%" + title + "%";
    sqlite3_bind_text(stmt, 1, title_pattern.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string book_title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        results.emplace_back(id, book_title);
    }
    sqlite3_finalize(stmt);
    return results;
}

std::vector<std::pair<int, std::string>> Database::searchBookByAuthor(const std::string& author) {
    const char* search_sql = "SELECT id, title FROM books WHERE author LIKE ?;";
    sqlite3_stmt* stmt;
    std::vector<std::pair<int, std::string>> results;
    if (sqlite3_prepare_v2(db, search_sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return results;
    }
    std::string author_pattern = "%" + author + "%";
    sqlite3_bind_text(stmt, 1, author_pattern.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string book_title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        results.emplace_back(id, book_title);
    }
    sqlite3_finalize(stmt);
    return results;
}

std::vector<std::tuple<int, std::string, std::string>> Database::getAllBooks() {
    const char* get_all_sql = "SELECT id, title, author FROM books;";
    sqlite3_stmt* stmt;
    std::vector<std::tuple<int, std::string, std::string>> results;
    if (sqlite3_prepare_v2(db, get_all_sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return results;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string book_title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string book_author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        results.emplace_back(id, book_title, book_author);
    }
    sqlite3_finalize(stmt);
    return results;
}
