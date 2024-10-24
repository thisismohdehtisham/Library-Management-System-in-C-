#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

class Book {
private:
    string title;
    string author;
    string isbn;
    bool available;

public:
    Book(string t, string a, string i)
        : title(t), author(a), isbn(i), available(true) {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return isbn; }
    bool isAvailable() const { return available; }

    void toggleAvailability() { available = !available; }

    void display() const {
        cout << "Title: " << title << "\nAuthor: " << author 
                  << "\nISBN: " << isbn << "\nStatus: " 
                  << (available ? "Available" : "Checked Out") << "\n\n";
    }
};

class Library {
private:
    vector<Book> books;
    string filename;

public:
    Library(string fname) : filename(fname) {
        loadBooks();
    }

    void addBook(const string& title, const string& author, const string& isbn) {
        books.emplace_back(title, author, isbn);
        saveBooks();
    }

    void searchBook(const string& query) {
        bool found = false;
        for (const auto& book : books) {
            if (book.getTitle().find(query) != string::npos ||
                book.getAuthor().find(query) != string::npos ||
                book.getISBN().find(query) != string::npos) {
                book.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No books found matching your query.\n";
        }
    }

    void checkoutBook(const string& isbn) {
        auto it = find_if(books.begin(), books.end(),
            [&isbn](const Book& b) { return b.getISBN() == isbn; });

        if (it != books.end()) {
            if (it->isAvailable()) {
                it->toggleAvailability();
                cout << "Book checked out successfully!\n";
                saveBooks();
            } else {
                cout << "Book is already checked out.\n";
            }
        } else {
            cout << "Book not found.\n";
        }
    }

    void returnBook(const string& isbn) {
        auto it = find_if(books.begin(), books.end(),
            [&isbn](const Book& b) { return b.getISBN() == isbn; });

        if (it != books.end()) {
            if (!it->isAvailable()) {
                it->toggleAvailability();
                cout << "Book returned successfully!\n";
                saveBooks();
            } else {
                cout << "Book is already in library.\n";
            }
        } else {
            cout << "Book not found.\n";
        }
    }

private:
    void loadBooks() {
        ifstream file(filename);
        string title, author, isbn, status;

        while (getline(file, title) && getline(file, author) && 
               getline(file, isbn) && getline(file, status)) {
            books.emplace_back(title, author, isbn);
            if (status == "0") {
                books.back().toggleAvailability();
            }
        }
    }

    void saveBooks() {
        ofstream file(filename);
        for (const auto& book : books) {
            file << book.getTitle() << '\n'
                 << book.getAuthor() << '\n'
                 << book.getISBN() << '\n'
                 << book.isAvailable() << '\n';
        }
    }
};

int main() {
    Library lib("library.txt");
    string choice, title, author, isbn, query;

    while (true) {
        cout << "\nLibrary Management System\n"
                  << "1. Add Book\n"
                  << "2. Search Book\n"
                  << "3. Checkout Book\n"
                  << "4. Return Book\n"
                  << "5. Exit\n"
                  << "Enter your choice: ";

        getline(cin, choice);

        if (choice == "1") {
            cout << "Enter title: ";
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter ISBN: ";
            getline(cin, isbn);
            lib.addBook(title, author, isbn);

        } else if (choice == "2") {
            cout << "Enter search term: ";
            getline(cin, query);
            lib.searchBook(query);

        } else if (choice == "3") {
            cout << "Enter ISBN to checkout: ";
            getline(cin, isbn);
            lib.checkoutBook(isbn);

        } else if (choice == "4") {
            cout << "Enter ISBN to return: ";
            getline(cin, isbn);
            lib.returnBook(isbn);

        } else if (choice == "5") {
            break;
        }
    }

    return 0;
}