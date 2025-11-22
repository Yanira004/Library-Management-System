#pragma once
#include "domain.h"
#include "repo.h"

class UndoAction {
public:
	virtual void doUndo() = 0;
	virtual ~UndoAction() {};
};

class AddUndo : public UndoAction {
	Book book;
	Repo* repo;
public:
	AddUndo(Repo* repo, const Book& b) : repo{ repo }, book{ b } {}
	void doUndo() override {
		repo->Delete_book(book.id);
	}
};

class DeleteUndo : public UndoAction {
	Book book;
	Repo* repo;
public:
	DeleteUndo(Repo* repo, const Book& b) : repo{ repo }, book{ b } {}
	void doUndo() override {
		repo->Add(book);
	}
};


class ModifyUndo : public UndoAction {
	Book original_book;
	Repo* repo;
public:
	ModifyUndo(Repo* repo, const Book& original) : repo{ repo }, original_book{ original } {}
	void doUndo() override {
		repo->modify(original_book);
	}
};

