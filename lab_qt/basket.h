#pragma once
#include <vector>
#include "domain.h"
#include "observer.h"

using namespace std;

class Basket : public Observable{
private:
	vector<Book> basket;
public:
	Basket();

	void add_to_basket(const Book& b);

	vector<Book>& get_basket();

	void empty_basket();

	void delete_by_title(string& title);

	void generate(int n, vector<Book>& list);

	int export_basket(const string& file);
};


