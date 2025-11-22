#pragma once
#include "repo.h"
#include "basket.h"
#include "undo.h"
#include "domain.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <map>

struct DTOcount {
    std::string genre;
    int count;
};

class Service {
    Repo* repo;
    Basket serv_basket;
    std::vector<std::unique_ptr<UndoAction>> undoActions;

public:
    Service(Repo* repo) : repo(repo) {}
    ~Service() { 
        undoActions.clear();
        delete repo; 
    }

    void undo();
    void clearUndoActions() { undoActions.clear(); }

    int Add_serv(std::string title, std::string author, std::string genre, int year, int id);
    int Delete_serv(int id);
    int Modify_serv(std::string title, std::string author, std::string genre, int year, int id);
    vector<Book>& get_list_serv();
    vector<Book> filter(vector<Book>& v, int year) const;
    void sort(std::vector<Book>& v, int order, function<bool(const Book&, const Book&)> cmp);
    int find_serv(int ID);
    const int validator(Book& b) const;

    void add_to_basket_serv(string& title);
    vector<Book>& get_basket_serv();
    void delete_basket_serv(string& title);
    void delete_whole_basket();
    void generate_basket_serv(int n);
    void export_basket_serv(const string& file);
    std::vector<DTOcount> get_genre_counts();
    Basket& getBasket() {
        return serv_basket;
    }
};