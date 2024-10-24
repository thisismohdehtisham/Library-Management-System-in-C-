# Library-Management-System-in-C++

This is a simple **Library Management System** built in C++. It allows users to perform various operations like adding, searching, checking out, returning, and deleting books. The system provides real-time book availability status updates and prevents duplicate ISBN entries for better data integrity.

## Features

- **Add Books**: Users can add new books by providing a title, author, and ISBN. Duplicate ISBN entries are validated and prevented.
- **Search Books**: Books can be searched using a case-insensitive query that checks the title, author, or ISBN.
- **Checkout & Return Books**: Users can check out and return books, and the system updates the book’s status in real-time.
- **Delete Books**: Allows users to delete books by ISBN.
- **View All Books**: Displays all the books in the library, along with their current availability.
- **Library Statistics**: Provides total books, available books, and checked-out books statistics.

## Data Persistence

The system uses a file (`library.txt`) to store book data persistently. Every time a book is added, removed, checked out, or returned, the file is updated to reflect the latest state of the library.

## Data Structures Used

- **Vector**: The library uses a `vector` to store the collection of books, allowing dynamic resizing and efficient operations like adding, deleting, and searching books.
- **String Manipulation**: To facilitate case-insensitive searching, helper functions are used to handle string transformations.

## How to Run

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/Library-Management-System.git

   g++ library.cpp -o library
   ./library

