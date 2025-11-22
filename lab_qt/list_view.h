#pragma once
#include "service.h"
#include <QAbstractListModel>
#include <QFont>
#include <QBrush>


class BookListModel : public QAbstractListModel {
	Q_OBJECT
private:
	Service* serv;
public:
	BookListModel(Service* serv, QObject* parent = nullptr) :QAbstractListModel(parent), serv(serv) {}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		return serv->get_list_serv().size();
	}

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
       if (!index.isValid() || index.row() >= serv->get_list_serv().size()) {
           return QVariant();
       }

       const Book& book = serv->get_list_serv()[index.row()];

       if (role == Qt::DisplayRole) {
           return QString::fromStdString(
               book.get_title() + " by " + book.get_author() + " (" +
               book.get_genre() + ", " + std::to_string(book.get_year()) + ")"
           );
       }
       else if (role == Qt::FontRole) {
           QFont font;
           font.setBold(true);
           return font;
       }
       else if (role == Qt::BackgroundRole) {
           if (book.get_year() > 2020) {
               return QBrush(Qt::darkBlue);
           }
       }
       else if (role == Qt::UserRole) {
           return book.get_id();
       }

       return QVariant();
    }


    void refresh() {
        beginResetModel();
        endResetModel();
    }
};