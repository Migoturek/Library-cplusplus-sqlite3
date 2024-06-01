#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <sqlite3.h>
#include <string>
#include <vector>

class Database {
public:
    Database(const std::string& db_name);
    ~Database();

    bool addBook(const std::string& title, const std::string& author);
    bool deleteBook(int id);
    std::vector<std::pair<int, std::string>> searchBookByTitle(const std::string& title);
    std::vector<std::pair<int, std::string>> searchBookByAuthor(const std::string& author);
    std::vector<std::tuple<int, std::string, std::string>> getAllBooks();

private:
    sqlite3* db;
};

#endif // DATABASE_HPP
