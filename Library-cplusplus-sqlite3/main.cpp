#include <iostream>
#include "database.hpp"

void printMenu() {
    std::cout << "1. Add book" << std::endl;
    std::cout << "2. Delete book" << std::endl;
    std::cout << "3. Search book by title" << std::endl;
    std::cout << "4. Search book by author" << std::endl;
    std::cout << "5. Display all books" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << "Choose an option: ";
}

int main() {
    Database db("books.db");
    int choice;

    while (true) {
        printMenu();
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            std::string title, author;
            std::cout << "Enter book title: ";
            std::getline(std::cin, title);
            std::cout << "Enter book author: ";
            std::getline(std::cin, author);
            if (db.addBook(title, author)) {
                std::cout << "Book added successfully!" << std::endl;
            }
            else {
                std::cout << "Failed to add book." << std::endl;
            }
        }
        else if (choice == 2) {
            int id;
            std::cout << "Enter book ID to delete: ";
            std::cin >> id;
            if (db.deleteBook(id)) {
                std::cout << "Book deleted successfully!" << std::endl;
            }
            else {
                std::cout << "Failed to delete book." << std::endl;
            }
        }
        else if (choice == 3) {
            std::string title;
            std::cout << "Enter title to search: ";
            std::cin.ignore(); // Ignorujemy znak nowej linii w buforze
            std::getline(std::cin, title);
            std::vector<std::pair<int, std::string>> books = db.searchBookByTitle(title);
            if (!books.empty()) {
                std::cout << "Book found: ID: " << books[0].first << ", Title: " << books[0].second << std::endl;
            }
            else {
                std::cout << "Book not found." << std::endl;
            }
        }
        else if (choice == 4) {
            std::string author;
            std::cout << "Enter author to search: ";
            std::cin.ignore(); // Ignorujemy znak nowej linii w buforze
            std::getline(std::cin, author);
            std::vector<std::pair<int, std::string>> books = db.searchBookByAuthor(author);
            if (!books.empty()) {
                std::cout << "Books found by " << author << ":" << std::endl;
                for (const auto& book : books) {
                    std::cout << "ID: " << book.first << ", Title: " << book.second << std::endl;
                }
            }
            else {
                std::cout << "No books found by " << author << "." << std::endl;
            }
        }
        else if (choice == 5) {
            std::vector<std::tuple<int, std::string, std::string>> books = db.getAllBooks();
            if (!books.empty()) {
                std::cout << "All books in the database:" << std::endl;
                for (const auto& book : books) {
                    std::cout << "ID: " << std::get<0>(book) << ", Title: " << std::get<1>(book) << ", Author: " << std::get<2>(book) << std::endl;
                }
            }
            else {
                std::cout << "No books in the database." << std::endl;
            }
        }
        else if (choice == 6) {
            std::cout << "Exiting..." << std::endl;
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    return 0;
}
