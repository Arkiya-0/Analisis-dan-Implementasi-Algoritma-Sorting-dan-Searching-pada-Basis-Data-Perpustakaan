#include <iostream>
#include <fstream>
#include <string>
using namespace std;


// ============================================
// CONSTANTS
// ============================================
const int MAX_BOOKS = 200;

// ============================================
// STRUCTURES
// ============================================
struct Book {
    string isbn;
    string title;
    string author;
    int year;
    string category;
};

// ============================================
// GLOBAL VARIABLES
// ============================================
Book allBooks[MAX_BOOKS];
int bookCount = 0;

// ============================================
// UTILITY FUNCTIONS
// ============================================

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

// ============================================
// FILE OPERATIONS
// ============================================

// Load books from CSV
void loadBooks() {
    ifstream file("library_books.csv");
    
    if (!file.is_open()) {
        cout << "📋 No existing database found. Starting fresh.\n";
        return;
    }
    
    string line;
    getline(file, line);  // Skip header
    
    bookCount = 0;
    
    while (getline(file, line) && bookCount < MAX_BOOKS) {
        if (line.empty()) continue;
        
        int pos = 0;
        int nextPos;
        
        nextPos = line.find(',', pos);
        allBooks[bookCount].isbn = line.substr(pos, nextPos - pos);
        pos = nextPos + 1;
        
        nextPos = line.find(',', pos);
        allBooks[bookCount].title = line.substr(pos, nextPos - pos);
        pos = nextPos + 1;
        
        nextPos = line.find(',', pos);
        allBooks[bookCount].author = line.substr(pos, nextPos - pos);
        pos = nextPos + 1;
        
        nextPos = line.find(',', pos);
        allBooks[bookCount].year = stoi(line.substr(pos, nextPos - pos));
        pos = nextPos + 1;
        
        allBooks[bookCount].category = line.substr(pos);
        
        bookCount++;
    }
    
    file.close();
    cout << "✅ Loaded " << bookCount << " books from database.\n";
}

// Save books to CSV
void saveBooks() {
    ofstream file("library_books.csv");
    
    // Write header
    file << "ISBN,Title,Author,Year,Category\n";
    
    // Write each book
    for (int i = 0; i < bookCount; i++) {
        file << allBooks[i].isbn << ","
             << allBooks[i].title << ","
             << allBooks[i].author << ","
             << allBooks[i].year << ","
             << allBooks[i].category << "\n";
    }
    
    file.close();
    cout << "✅ Database saved successfully!\n";
}

// ============================================
// SPREADSHEET OPERATIONS (CRUD)
// ============================================

// CREATE - Add new book
void addBook() {
    clearScreen();
    
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║         ADD NEW BOOK                   ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    
    if (bookCount >= MAX_BOOKS) {
        cout << "❌ Database is full! Maximum " << MAX_BOOKS << " books.\n";
        pauseScreen();
        return;
    }
    
    Book newBook;
    
    cout << "📖 Enter book details:\n\n";
    
    cout << "ISBN: ";
    cin.ignore();
    getline(cin, newBook.isbn);
    
    // Check for duplicate ISBN
    for (int i = 0; i < bookCount; i++) {
        if (allBooks[i].isbn == newBook.isbn) {
            cout << "❌ Book with this ISBN already exists!\n";
            pauseScreen();
            return;
        }
    }
    
    cout << "Title: ";
    getline(cin, newBook.title);
    
    cout << "Author: ";
    getline(cin, newBook.author);
    
    cout << "Year: ";
    cin >> newBook.year;
    cin.ignore();
    
    cout << "\nCategory:\n";
    cout << "1. Programming\n";
    cout << "2. Computer Science\n";
    cout << "3. Science\n";
    cout << "4. Literature\n";
    cout << "5. History\n";
    cout << "Choice: ";
    
    int catChoice;
    cin >> catChoice;
    cin.ignore();
    
    switch (catChoice) {
        case 1: newBook.category = "Programming"; break;
        case 2: newBook.category = "Computer Science"; break;
        case 3: newBook.category = "Science"; break;
        case 4: newBook.category = "Literature"; break;
        case 5: newBook.category = "History"; break;
        default: newBook.category = "Other"; break;
    }
    
    allBooks[bookCount] = newBook;
    bookCount++;
    
    saveBooks();
    
    cout << "\n════════════════════════════════════════\n";
    cout << "✅ Book added successfully!\n";
    cout << "Total books in database: " << bookCount << "\n";
    cout << "════════════════════════════════════════\n";
    
    pauseScreen();
}

// READ - View all books
void viewAllBooks() {
    clearScreen();
    
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║      BOOK DATABASE (SPREADSHEET)       ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    
    if (bookCount == 0) {
        cout << "📋 Database is empty. Add some books first!\n";
        pauseScreen();
        return;
    }
    
    cout << "Total books: " << bookCount << "\n\n";
    
    // Display in table format
    cout << "────────────────────────────────────────────────────────────────────────\n";
    cout << " #  | ISBN          | Title                    | Author          | Year\n";
    cout << "────────────────────────────────────────────────────────────────────────\n";
    
    for (int i = 0; i < bookCount; i++) {
        cout.width(3);
        cout << left << (i + 1) << " | ";
        
        cout.width(13);
        cout << left << allBooks[i].isbn << " | ";
        
        string title = allBooks[i].title;
        if (title.length() > 24) title = title.substr(0, 21) + "...";
        cout.width(24);
        cout << left << title << " | ";
        
        string author = allBooks[i].author;
        if (author.length() > 15) author = author.substr(0, 12) + "...";
        cout.width(15);
        cout << left << author << " | ";
        
        cout << allBooks[i].year << "\n";
    }
    
    cout << "────────────────────────────────────────────────────────────────────────\n";
    
    pauseScreen();
}

// UPDATE - Edit existing book
void editBook() {
    clearScreen();
    
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║         EDIT BOOK                      ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    
    if (bookCount == 0) {
        cout << "❌ No books to edit!\n";
        pauseScreen();
        return;
    }
    
    cout << "Enter ISBN of book to edit: ";
    string isbn;
    cin.ignore();
    getline(cin, isbn);
    
    int index = -1;
    for (int i = 0; i < bookCount; i++) {
        if (allBooks[i].isbn == isbn) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        cout << "❌ Book not found!\n";
        pauseScreen();
        return;
    }
    
    cout << "\n📖 Current details:\n";
    cout << "Title: " << allBooks[index].title << "\n";
    cout << "Author: " << allBooks[index].author << "\n";
    cout << "Year: " << allBooks[index].year << "\n";
    cout << "Category: " << allBooks[index].category << "\n\n";
    
    cout << "Enter new details (press Enter to keep current):\n\n";
    
    string input;
    
    cout << "New Title: ";
    getline(cin, input);
    if (!input.empty()) allBooks[index].title = input;
    
    cout << "New Author: ";
    getline(cin, input);
    if (!input.empty()) allBooks[index].author = input;
    
    cout << "New Year: ";
    getline(cin, input);
    if (!input.empty()) allBooks[index].year = stoi(input);
    
    cout << "New Category: ";
    getline(cin, input);
    if (!input.empty()) allBooks[index].category = input;
    
    saveBooks();
    
    cout << "\n✅ Book updated successfully!\n";
    pauseScreen();
}

// DELETE - Remove book
void deleteBook() {
    clearScreen();
    
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║         DELETE BOOK                    ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    
    if (bookCount == 0) {
        cout << "❌ No books to delete!\n";
        pauseScreen();
        return;
    }
    
    cout << "Enter ISBN of book to delete: ";
    string isbn;
    cin.ignore();
    getline(cin, isbn);
    
    int index = -1;
    for (int i = 0; i < bookCount; i++) {
        if (allBooks[i].isbn == isbn) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        cout << "❌ Book not found!\n";
        pauseScreen();
        return;
    }
    
    cout << "\n⚠️  You are about to delete:\n";
    cout << "Title: " << allBooks[index].title << "\n";
    cout << "Author: " << allBooks[index].author << "\n\n";
    
    cout << "Are you sure? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        // Shift all books after deleted one
        for (int i = index; i < bookCount - 1; i++) {
            allBooks[i] = allBooks[i + 1];
        }
        bookCount--;
        
        saveBooks();
        
        cout << "\n✅ Book deleted successfully!\n";
        cout << "Total books remaining: " << bookCount << "\n";
    } else {
        cout << "\n❌ Deletion cancelled.\n";
    }
    
    pauseScreen();
}

// Search in spreadsheet
void searchBook() {
    clearScreen();
    
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║         SEARCH BOOK                    ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    
    cout << "Enter search keyword (title or author): ";
    string keyword;
    cin.ignore();
    getline(cin, keyword);
    
    cout << "\n📖 Search Results:\n";
    cout << "────────────────────────────────────────\n\n";
    
    bool found = false;
    
    for (int i = 0; i < bookCount; i++) {
        if (allBooks[i].title.find(keyword) != string::npos ||
            allBooks[i].author.find(keyword) != string::npos) {
            
            cout << "ISBN: " << allBooks[i].isbn << "\n";
            cout << "Title: " << allBooks[i].title << "\n";
            cout << "Author: " << allBooks[i].author << "\n";
            cout << "Year: " << allBooks[i].year << "\n";
            cout << "Category: " << allBooks[i].category << "\n\n";
            
            found = true;
        }
    }
    
    if (!found) {
        cout << "❌ No books found.\n";
    }
    
    pauseScreen();
}

// Export to different format
void exportData() {
    clearScreen();
    
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║         EXPORT DATA                    ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    
    cout << "Export format:\n";
    cout << "1. CSV (Comma-separated)\n";
    cout << "2. TXT (Tab-separated)\n";
    cout << "3. Formatted Report\n";
    cout << "Choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 1) {
        saveBooks();
        cout << "\n✅ Exported to: library_books.csv\n";
    } else if (choice == 2) {
        ofstream file("library_books.txt");
        file << "ISBN\tTitle\tAuthor\tYear\tCategory\n";
        for (int i = 0; i < bookCount; i++) {
            file << allBooks[i].isbn << "\t"
                 << allBooks[i].title << "\t"
                 << allBooks[i].author << "\t"
                 << allBooks[i].year << "\t"
                 << allBooks[i].category << "\n";
        }
        file.close();
        cout << "\n✅ Exported to: library_books.txt\n";
    } else if (choice == 3) {
        ofstream file("library_report.txt");
        file << "═══════════════════════════════════════\n";
        file << "       LIBRARY BOOK REPORT\n";
        file << "═══════════════════════════════════════\n\n";
        file << "Total Books: " << bookCount << "\n\n";
        
        for (int i = 0; i < bookCount; i++) {
            file << (i + 1) << ". " << allBooks[i].title << "\n";
            file << "   Author: " << allBooks[i].author << "\n";
            file << "   Year: " << allBooks[i].year << "\n";
            file << "   Category: " << allBooks[i].category << "\n";
            file << "   ISBN: " << allBooks[i].isbn << "\n\n";
        }
        
        file.close();
        cout << "\n✅ Exported to: library_report.txt\n";
    }
    
    pauseScreen();
}

// ============================================
// MAIN MENU
// ============================================

void mainMenu() {
    while (true) {
        clearScreen();
        
        cout << "\n╔════════════════════════════════════════╗\n";
        cout << "║    SPREADSHEET MANAGER (DATABASE)      ║\n";
        cout << "╚════════════════════════════════════════╝\n\n";
        
        cout << "📊 Books in database: " << bookCount << "\n";
        cout << "📁 File: library_books.csv\n\n";
        
        cout << "────────────────────────────────────────\n";
        cout << "CRUD OPERATIONS:\n";
        cout << "1. ➕ Add New Book (CREATE)\n";
        cout << "2. 📖 View All Books (READ)\n";
        cout << "3. ✏️  Edit Book (UPDATE)\n";
        cout << "4. ❌ Delete Book (DELETE)\n";
        cout << "\nOTHER OPERATIONS:\n";
        cout << "5. 🔍 Search Book\n";
        cout << "6. 📤 Export Data\n";
        cout << "7. 🚪 Exit\n";
        cout << "────────────────────────────────────────\n";
        cout << "\nChoice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                viewAllBooks();
                break;
            case 3:
                editBook();
                break;
            case 4:
                deleteBook();
                break;
            case 5:
                searchBook();
                break;
            case 6:
                exportData();
                break;
            case 7:
                cout << "\n💾 Saving database...\n";
                saveBooks();
                cout << "👋 Goodbye!\n\n";
                return;
            default:
                cout << "\n❌ Invalid choice!\n";
                pauseScreen();
        }
    }
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "\n";
    cout << "════════════════════════════════════════\n";
    cout << "  SPREADSHEET MANAGER v1.0\n";
    cout << "  Manage Library Book Database\n";
    cout << "════════════════════════════════════════\n\n";
    
    loadBooks();
    
    pauseScreen();
    
    mainMenu();
    
    return 0;
}