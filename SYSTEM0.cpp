#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>
#include <limits>
using namespace std;

struct Book {
    string isbn;
    string author;
    int tahun;
    string judul;
    string genre;
    string rak; //POSISI
};

//DATABASE
vector<Book> libraryDB;

//IGNORE SPASI 
string trim(const string &s) {
    size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start])) start++;

    size_t end = s.size();
    while (end > start && isspace((unsigned char)s[end - 1])) end--;

    return s.substr(start, end - start);
}


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
    
}

//DOWNLOAD DATA
void downloaddata(const string &url, const string &outputFile){
    string command = "curl -L \"" + url + "\" -o " + outputFile + " --silent";
    system(command.c_str());
    cout << "Data diunduh dari Google Sheets...\n";
}

//LOAD DATA ( UNFINISH )
void loadLibraryData(const string &filename) {
    string googleurl = "https://script.google.com/macros/s/AKfycby17d0_-uWYrZp_1J3nKfV0cjwVlZct_1h8nOt7gtgcbh_6hL_YNsB-85BGqAWgVHvi/exec";
    cout << "Mengambil Data Dari Spreadshet...\n";
    downloaddata(googleurl, filename);
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Gagal membuka file!" << endl;
        return;
    }
    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        if(line.empty())continue;

        stringstream ss(line);
        string isbn, author, tahun, judul, genre, rak;

        getline(ss, isbn, ',');
        getline(ss, author, ',');
        getline(ss, tahun, ',');
        getline(ss, judul, ',');
        getline(ss, genre, ',');
        getline(ss, rak, ',');
        
        //TRIM
        isbn =trim(isbn);
        isbn  = trim(isbn);
        author = trim(author);
        tahun = trim(tahun);
        judul = trim(judul);
        genre = trim(genre);
        rak = trim(rak);

        Book b;
        b.isbn = trim(isbn);
        b.author = trim(author);
        b.judul = trim(judul);
        b.genre = trim(genre);
        b.rak = trim(rak);

        tahun = trim(tahun);

        if (tahun.empty() || !isdigit(tahun[0])) b.tahun = 0;
        else {
            try { b.tahun = stoi(tahun); }
            catch (...) { b.tahun = 0; }
        }


        libraryDB.push_back(b);
    }

    cout << "Data perpustakaan berhasil dimuat: " 
         << libraryDB.size() << " buku.\n";

    writeLog("Load Library Data: " + filename);
}

//SEARCH ENGINE
void searchBook() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
    if (choice == 4) key = favGenre;
    else {
        cout << "Masukkan kata kunci: ";
        getline(cin, key);
    }

    cout << "\n=== HASIL PENCARIAN ===\n";
    bool found = false;

    for (auto &b : libraryDB) {
        string target;
        if (choice == 1) target = b.judul;
        else if (choice == 2) target = b.author;
        else if (choice == 3) target = b.genre;
        else if (choice == 4) target = b.genre;
        else return;

        if (target.find(key) != string::npos) {
            cout << "\nISBN  : " << b.isbn
                 << "\nJudul : " << b.judul
                 << "\nAuthor: " << b.author
                 << "\nTahun : " << b.tahun
                 << "\nGenre : " << b.genre
                 << "\nRak   : " << b.rak << "\n";
            found = true;
        }
    }

    if (!found) cout << "Tidak ada buku yang cocok.\n";

    writeLog("Search: " + key);
}

void menu() {
    int pilihan = 0;

    while (true) {
        cout << "\n=== MENU UTAMA ===\n";
        cout << "1. Pencarian Buku\n";
        cout << "2. Keluar\n";
        cout << "Pilihan: ";

        cin >> pilihan;

        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (pilihan == 1) searchBook();
        else if (pilihan == 2) {
            cout << "Keluar...\n";
            writeLog("Logout");
            break;
        }
    }
}

int main() {
    cout << "===== SISTEM PERPUSTAKAAN C++ =====\n";

    cout << "Masukkan username: ";
    getline(cin, currentUser);

    cout << "Masukkan genre favorit Anda: ";
    getline(cin, favGenre);

    writeLog("Login User");

    loadLibraryData("library_books_200.csv");

    menu();
    return 0;
}