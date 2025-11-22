#include "repo.h"
#include <algorithm>
#include <fstream>
#include <sstream>

vector<Book>& Repo::get_list() {
    return Book_list;
}

int Repo::Add(const Book& b) {
    if (find_id(b.id) != -1)
        return 0;
    Book_list.push_back(b);
    save_to_file();
    return 1;
}

int Repo::find_id(int ID) const {
    auto pos = find_if(Book_list.begin(), Book_list.end(), [&](const auto& Book) {
        return Book.id == ID;
        });

    if (pos != Book_list.end()) {
        return static_cast<int>(pos - Book_list.begin());
    }
    return -1;
}

int Repo::modify(const Book& b) {
    int pos = find_id(b.id);
    if (pos == -1) return 0;
    Book_list[pos] = b;
    save_to_file();
    return 1;
}

int Repo::Delete_book(int id) {
    auto pos = find_if(Book_list.begin(), Book_list.end(), [&](const auto& Book) {
        return Book.id == id;
        });

    if (pos != Book_list.end()) {
        Book_list.erase(pos);
        save_to_file();
        return 1;
    }
    return 0;
}

const int Repo::find_book_title(const string& title) const {
    auto it = find_if(Book_list.begin(), Book_list.end(), [&](const auto& book) {
        return book.title == title;
        });
    if (it != Book_list.end()) {
        return static_cast<int>(it - Book_list.begin());
    }
    return -1;
}



void FileRepo::load_from_file() {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        return;
    }

    Book_list.clear();
    std::string line;

    while (std::getline(fin, line)) {
        std::stringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;

        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() == 5) {
            int id = std::stoi(tokens[0]);
            int year = std::stoi(tokens[4]);

            Book_list.emplace_back(
                tokens[1],  // title
                tokens[2],  // author
                tokens[3],  // genre
                year,
                id
            );

        }
    }
}

void FileRepo::save_to_file() {
    std::ofstream fout(filename);

    for (const auto& book : Book_list) {
        fout << book.id << "|"
            << book.title << "|"
            << book.author << "|"
            << book.genre << "|"
            << book.year << "\n";
    }
    fout.close();
}