#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <limits>

using namespace std;

// Helper function to convert string to lowercase
string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), 
             [](unsigned char c){ return tolower(c); });
    return str;
}

// Helper function to clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

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
        cout << "\n----------------------------------------\n";
        cout << "Title: " << title << "\n"
             << "Author: " << author << "\n"
             << "ISBN: " << isbn << "\n"
             << "Status: " << (available ? "Available" : "Checked Out") << "\n";
        cout << "----------------------------------------\n";
    }

    // Helper method for case-insensitive search
    bool matchesQuery(const string& query) const {
        string lowerQuery = toLower(query);
        return toLower(title).find(lowerQuery) != string::npos ||
               toLower(author).find(lowerQuery) != string::npos ||
               toLower(isbn).find(lowerQuery) != string::npos;
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
        // Check if ISBN already exists
        if (findBookByISBN(isbn) != books.end()) {
            cout << "\nError: A book with this ISBN already exists!\n";
            return;
        }

        books.emplace_back(title, author, isbn);
        saveBooks();
        cout << "\nBook added successfully!\n";
    }

    void searchBook(const string& query) {
        bool found = false;
        cout << "\nSearch Results for '" << query << "':\n";
        
        for (const auto& book : books) {
            if (book.matchesQuery(query)) {
                book.display();
                found = true;
            }
        }
        
        if (!found) {
            cout << "\nNo books found matching your query.\n";
        }
    }

    void checkoutBook(const string& isbn) {
        auto it = findBookByISBN(isbn);
            
        if (it != books.end()) {
            if (it->isAvailable()) {
                it->toggleAvailability();
                cout << "\nBook checked out successfully!\n";
                saveBooks();
            } else {
                cout << "\nBook is already checked out.\n";
            }
        } else {
            cout << "\nBook not found.\n";
        }
    }

    void returnBook(const string& isbn) {
        auto it = findBookByISBN(isbn);
            
        if (it != books.end()) {
            if (!it->isAvailable()) {
                it->toggleAvailability();
                cout << "\nBook returned successfully!\n";
                saveBooks();
            } else {
                cout << "\nError: Book is already in library.\n";
            }
        } else {
            cout << "\nError: Book not found.\n";
        }
    }

    // New Delete Book Method
    void deleteBook(const string& isbn) {
        auto it = findBookByISBN(isbn);

        if (it != books.end()) {
            books.erase(it);
            saveBooks();
            cout << "\nBook deleted successfully!\n";
        } else {
            cout << "\nError: Book not found.\n";
        }
    }

    void displayAllBooks() const {
        if (books.empty()) {
            cout << "\nNo books in the library.\n";
            return;
        }
        
        cout << "\n=== All Books in Library ===\n";
        for (const auto& book : books) {
            book.display();
        }
    }

    int getTotalBooks() const {
        return books.size();
    }

    int getAvailableBooks() const {
        return count_if(books.begin(), books.end(), 
            [](const Book& b) { return b.isAvailable(); });
    }

private:
    vector<Book>::iterator findBookByISBN(const string& isbn) {
        return find_if(books.begin(), books.end(),
            [&isbn](const Book& b) { 
                return toLower(b.getISBN()) == toLower(isbn); 
            });
    }

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

void displayMenu() {
    cout << "\n=== Library Management System ===\n"
         << "1. Add Book\n"
         << "2. Search Book\n"
         << "3. Checkout Book\n"
         << "4. Return Book\n"
         << "5. Delete Book\n"  // Added Delete Book option
         << "6. Display All Books\n"
         << "7. View Library Statistics\n"
         << "8. Exit\n"
         << "===============================\n"
         << "Enter your choice: ";
}

int main() {
    Library lib("library.txt");
    string choice, title, author, isbn, query;

    cout << "Welcome to Library Management System\n";

    while (true) {
        displayMenu();
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
            cout << "Enter search term (case-insensitive): ";
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
            
        } else if (choice == "5") {  // Delete book option
            cout << "Enter ISBN to delete: ";
            getline(cin, isbn);
            lib.deleteBook(isbn);

        } else if (choice == "6") {
            lib.displayAllBooks();
            
        } else if (choice == "7") {
            int total = lib.getTotalBooks();
            int available = lib.getAvailableBooks();
            cout << "\n=== Library Statistics ===\n"
                 << "Total Books: " << total << "\n"
                 << "Available Books: " << available << "\n"
                 << "Checked Out Books: " << (total - available) << "\n"
                 << "========================\n";
            
        } else if (choice == "8") {
            cout << "\nThank you for using the Library Management System!\n";
            break;
        } else {
            cout << "\nInvalid choice. Please try again.\n";
        }
    }

    return 0;
}
