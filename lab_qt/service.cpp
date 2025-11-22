#include "service.h"
#include "domain.h"
#include "exceptions.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>

using namespace std;


void Service::undo() {
    if (undoActions.empty()) {
        throw std::exception("No more undoes available");
    }
    undoActions.back()->doUndo();
    undoActions.pop_back();

}


int Service::Add_serv(string title, string author, string genre, int year, int id) {
    Book b(title, author, genre, year, id);

    if (!validator(b)) {
        throw ValidationException("Invalid input");
    }
    this->repo->Add(b);
    undoActions.push_back(std::make_unique<AddUndo>(repo, b));
    return 1;
}



int Service::Modify_serv(string title, string author, string genre, int year, int id) {
    Book b = Book(title, author, genre, year, id);

    int position = this->repo->find_id(id);

    if (position == -1) {
        throw BookNotFoundException("The book doesn't exist");
    }

    int verif;
    verif = validator(b);
    if (verif == 0) {
        throw ValidationException("Invalid input");
    }

    Book original = this->repo->get_list()[position];

    verif = this->repo->modify(b);

    undoActions.push_back(std::make_unique<ModifyUndo>(repo, original));

    return 1;
}

int Service::Delete_serv(int id) {
    if (this->repo->get_list().empty()) {
        throw EmptyListException("The library is empty");
    }

    int position = this->repo->find_id(id);
    if (position == -1) {  // Add this check!
        throw BookNotFoundException("The book doesn't exist");
    }

    Book book_undo = this->repo->get_list()[position];  // Safe now
    int verif = this->repo->Delete_book(id);

    undoActions.push_back(std::make_unique<DeleteUndo>(repo, book_undo));
    return 1;
}

vector<Book>& Service::get_list_serv() {
    return this->repo->get_list();
}

vector<Book> Service::filter(vector<Book>& v, int year) const {
    vector<Book> filtered_books;

    for (const auto& book : v) {
        if (book.year >= year) {
            filtered_books.push_back(book);
        }
    }
    return filtered_books;
}

void Service::sort(std::vector<Book>& v, int order, function<bool(const Book&, const Book&)> cmp) {
    if (order == 1) {
        std::sort(v.begin(), v.end(), [cmp](const Book& a, const Book& b) {
            return cmp(b, a);
            });
    }
    else {
        std::sort(v.begin(), v.end(), cmp);
    }
}



int Service::find_serv(int ID) {
    int position = this->repo->find_id(ID);
    return position;
}

const int Service::validator(Book& b) const {
    vector<std::string> genres = { "SF", "romance", "history", "drama", "action", "fantasy", "YA" };

    if (b.year <= 0 or b.year > 2025) {
        return 0;
    }
    if (b.title.empty() == true) {
        return 0;
    }
    if (b.author.empty() == true) {
        return 0;
    }
    if (b.genre.empty() == true) {
        return 0;
    }
    if (std::find(genres.begin(), genres.end(), b.genre) == genres.end()) {
        return 0;
    }

    return 1;
}


//=========================================================================================




void Service::add_to_basket_serv(string& title) {
    int position = this->repo->find_book_title(title);
    if (position == -1) {
        throw BookNotFoundException("The book you are looking for is not in the library");
    }
    this->serv_basket.add_to_basket(this->repo->get_list()[position]);
}


vector<Book>& Service::get_basket_serv() {
    return this->serv_basket.get_basket();
}


void Service::delete_basket_serv(string& title) {
    vector<Book>& basket = this->serv_basket.get_basket();
    if (basket.size() == 0) {
        throw EmptyListException("The basket is already empty");
    }

    auto it = find_if(basket.begin(), basket.end(), [&](const Book& b) {
        return b.get_title() == title;
        });

    if (it == basket.end()) {
        throw BookNotFoundException("The book you are looking for is not in the basket");
    }
    this->serv_basket.delete_by_title(title);
}


void Service::delete_whole_basket() {
    if (this->serv_basket.get_basket().size() == 0) {
        throw EmptyListException("The basket is already empty");
    }
    this->serv_basket.empty_basket();
}


void Service::generate_basket_serv(int n) {
    /*if (this->serv_basket.get_basket().size() == 0) {
        throw EmptyListException("The basket is already empty");
    }*/

    this->serv_basket.generate(n, this->repo->get_list());
}


void Service::export_basket_serv(const string& file) {
    if (this->serv_basket.get_basket().size() == 0) {
        throw EmptyListException("The basket is already empty");
    }

    if (file.length() < 5 or (file.substr(file.length() - 4) != ".csv" && file.substr(file.length() - 5) != ".html")) {
        throw FileException("Invalid filename");
    }

    int v = this->serv_basket.export_basket(file);
    if (v == 0) {
        throw FileException("File could not be opened");
    }

}


std::vector<DTOcount> Service::get_genre_counts() {
    std::map<std::string, int> freq_map;

    // Work with the existing non-const get_list() by making a copy
    std::vector<Book> books = repo->get_list();

    for (const auto& book : books) {
        freq_map[book.get_genre()]++;
    }

    std::vector<DTOcount> result;
    for (const auto& entry : freq_map) {
        result.push_back({ entry.first, entry.second });
    }

    return result;
}


