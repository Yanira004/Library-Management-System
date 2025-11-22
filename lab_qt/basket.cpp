#include "basket.h"
#include "domain.h"
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <algorithm>

Basket::Basket() {
	basket = vector<Book>{};
	basket.reserve(30);
}


void Basket::add_to_basket(const Book& b) {
	basket.push_back(b);
	notify();
}


std::vector<Book>& Basket::get_basket() {
	return basket;
}


void Basket::empty_basket() {
	basket.clear();
	notify();
}


void Basket::delete_by_title(string& title) {
	auto it = remove_if(basket.begin(), basket.end(), [&](const Book& b) { return b.get_title() == title; });
	if (it != basket.end()) {
		basket.erase(it, basket.end());
		notify();
	}
}


void Basket::generate(int n, vector<Book>& list) {
	this->empty_basket();
	vector<Book> random_number;
	for (int i = 0; i < list.size(); i++)
		random_number.push_back(list[i]);
	random_device rand;
	mt19937 g(rand());
	shuffle(random_number.begin(), random_number.end(), g);
	for (int i = 0; i < n; i++) {
		add_to_basket(random_number[i]);
	}
	notify();
}


int Basket::export_basket(const string& file) {
	ofstream fout(file);
	if (!fout.is_open()) {
		return 0;
	}

	for (const auto& book : basket) {
		fout << book.id << " | " << book.title << " by "
			<< book.author << ", " << book.genre
			<< ", year " << book.year << endl;
	}

	fout.close();
	return 1;
}
