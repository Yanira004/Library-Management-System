#include "domain.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int copy_count = 0;

Book::Book(const Book& x) : title(x.title), author(x.author), genre(x.genre), year(x.year), id(x.id) {
	copy_count++;
	//std::cout << "Copy made! Total copies: " << copy_count << std::endl;
}

Book::Book() : title(""), author(""), genre(""), year(-1), id(-1) {}


Book::Book(const string& t, const string& a, const string& g, int y, int i) : title(t), author(a), genre(g), year(y), id(i) {}



Book& Book::operator=(const Book& x) {
	if (this == &x) return *this;

	this->title = x.title;
	this->genre = x.genre;
	this->year = x.year;
	this->author = x.author;
	this->id = x.id;
	return *this;
}

Book::~Book() {}


bool Book::operator==(const Book& other) const {
	return (this->id == other.id &&
		this->title == other.title &&
		this->author == other.author &&
		this->genre == other.genre &&
		this->year == other.year);
}

void Book::print_book() const {
	cout << this->id << " | " << this->title << " by " << this->author << ", " << this->genre << ", year " << this->year << endl;

}

int Book::get_id() const {
	return this->id;
}

int Book::get_year() const {
	return this->year;
}

string Book::get_title() const {
	return this->title;
}
string Book::get_author() const {
	return this->author;
}

string Book::get_genre() const {
	return this->genre;
}
