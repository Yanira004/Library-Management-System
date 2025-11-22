#pragma once
#include "domain.h"
#include "exceptions.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Repo {
protected:
    vector<Book> Book_list;
    string filename;

public:
    Repo() = default;
    Repo(const string& filename) : filename(filename) {}

    virtual ~Repo() = default;

    virtual void load_from_file() = 0;
    virtual void save_to_file() = 0;

    vector<Book>& get_list();
    int Add(const Book& x);
    int find_id(int id) const;
    int Delete_book(int id);
    int modify(const Book& b);
    const int find_book_title(const string& title) const;
};



class InMemoryRepo : public Repo {
public:
    InMemoryRepo() : Repo() {}
    void load_from_file() override {}
    void save_to_file() override {}
};



class FileRepo : public Repo {
public:
    FileRepo(const std::string& filename) : Repo(filename) {
        load_from_file();
    }

    void load_from_file() override;
    void save_to_file() override;
};