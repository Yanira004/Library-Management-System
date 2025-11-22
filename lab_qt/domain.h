#pragma once
#include <string>
using std::string;

class Book {
public:
	std::string title, author, genre;

	int year, id;

	Book();
	Book(const string& t, const string& a, const string& g, int y, int i);
	Book(const Book& x);
	Book& operator=(const Book& x);
	~Book();

	void print_book() const;
	int get_id() const;
	int get_year() const;
	string get_title() const;
	string get_author() const;
	string get_genre() const;

	bool operator==(const Book& other) const;

	
};