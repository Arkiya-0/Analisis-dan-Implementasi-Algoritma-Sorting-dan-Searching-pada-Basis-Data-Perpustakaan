#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

struct Book {
    string isbn;
    string author;
    int tahun;
    string judul;
    string genre;
};

//DATABASE
vector<Book> libraryDB;

//USER INFO
string currentUser;
string favGenre;

//LOG
void writeLog(const string &action) {
    ofstream logFile("system_log.txt", ios::app);
    if (logFile.is_open()) {
        logFile << "USER: " << currentUser 
                << " | ACTION: " << action << endl;
    }
    logFile.close();
}

//LOAD DATA ( UNFINISH )
void loadLibraryData(const string &filename) {
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cout << "Gagal membuka file!" << endl;
        return;
    }

    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string isbn, author, year, titjudulle, genre;

        getline(ss, isbn, ',');
        getline(ss, author, ',');
        getline(ss, year, ',');
        getline(ss, judul, ',');
        getline(ss, genre, ',');

        Book b;
        b.isbn = isbn;
        b.author = author;
        b.tahun = stoi(year);
        b.judul = judul;
        b.genre = genre;

        libraryDB.push_back(b);
    }

    file.close();
    cout << "Data perpustakaan berhasil dimuat: " 
         << libraryDB.size() << " buku.\n";

    writeLog("Load Library Data: " + filename);
}

//SEARCH ENGINE
void searchBook() {
    int choice;
    cout << "\n=== MENU SEARCH ===\n";
    cout << "1. Cari berdasarkan Judul\n";
    cout << "2. Cari berdasarkan Author\n";
    cout << "3. Cari berdasarkan Genre\n";
    cout << "4. Cari berdasarkan Genre Favorit Anda\n";
    cout << "Pilihan: ";
    cin >> choice;
    cin.ignore();

    string key;
    cout << "Masukkan kata kunci: ";
    getline(cin, key);

    cout << "\n=== HASIL PENCARIAN ===\n";
    bool found = false;

    for (auto &b : libraryDB) {
        string target;

        if (choice == 1) target = b.title;
        else if (choice == 2) target = b.author;
        else if (choice == 3) target = b.genre;
        else if (choice == 4) target = favGenre;
        else return;

        if (target.find(key) != string::npos) {
            cout << "\nISBN  : " << b.isbn << endl;
            cout << "Judul : " << b.title << endl;
            cout << "Author: " << b.author << endl;
            cout << "Tahun : " << b.tahun << endl;
            cout << "Genre : " << b.genre << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "Tidak ada buku yang cocok.\n";
    }

    writeLog("Search Book with keyword: " + key);
}

//MENU
void menu() {
    int pilihan;
    do {
        cout << "\n=== MENU UTAMA ===\n";
        cout << "1. Pencarian Buku\n";
        cout << "2. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan;

        cin.ignore();

        switch (pilihan) {
            case 1:
                searchBook();
                break;
            case 2:
                cout << "Keluar...\n";
                writeLog("Logout");
                break;
        }
    } while (pilihan != 2);
}


int main() {
    cout << "===== SISTEM PERPUSTAKAAN C++ =====\n";
    
    cout << "Masukkan username: ";
    getline(cin, currentUser);

    cout << "Masukkan genre favorit Anda: ";
    getline(cin, favGenre);

    writeLog("Login User");

    // Load CSV
    loadLibraryData("library_books_200.csv");

    // Masuk Menu
    menu();

    return 0;
}
