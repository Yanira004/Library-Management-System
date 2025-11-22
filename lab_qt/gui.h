#pragma once
#include <QWidget>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListView>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QTableWidget>
#include "service.h"
#include "tests.h"
#include "basket_gui.h"
#include "list_view.h"

class GUI : public QWidget {
public:
    GUI(Service* serv) : serv(serv) {
        run_tests();
        my_window();
        my_basket_window();
        add_books_from_file();
        connect_it();
        genre_buttons();
        window->move(100, 100);
        basket_window->move(100, 400);
        window->show();
    }

    ~GUI() override {
        delete window;
    }

private:
    Service* serv;
    QListView* booklist = new QListView(this);
    QListWidget* bookbasket = new QListWidget(this);
    BookListModel* bookModel = nullptr;

    QLineEdit* titlebox = new QLineEdit(this);
    QLineEdit* authorbox = new QLineEdit(this);
    QLineEdit* yearbox = new QLineEdit(this);
    QLineEdit* genrebox = new QLineEdit(this);
    QLineEdit* idbox = new QLineEdit(this);
    QLineEdit* titlebasketbox = new QLineEdit(this);

    QPushButton* addbutton = new QPushButton("Add book", this);
    QPushButton* deletebutton = new QPushButton("Delete book", this);
    QPushButton* deletebasket = new QPushButton("Delete book from basket", this);
    QPushButton* addbasket = new QPushButton("Add book to basket", this);
    QPushButton* generatebasket = new QPushButton("Generate basket", this);
    QPushButton* emptybasket = new QPushButton("Clear basket", this);
    QPushButton* exportbasket = new QPushButton("Export basket", this);

    QWidget* window = new QWidget;
    QWidget* basket_window = new QWidget;

    QPushButton* undobutton = new QPushButton("Undo", window);
    QPushButton* exitbutton = new QPushButton("Exit", window);
    QPushButton* showbutton = new QPushButton("Show all books", window);
    QPushButton* filteryearbutton = new QPushButton("Show books after a year", this);
    QPushButton* sortbutton = new QPushButton("Sort books after title", this);
    QPushButton* showbasket = new QPushButton("Show basket", this);

    QHBoxLayout* genreButtonsLayout;
    std::map<std::string, QPushButton*> genreButtons;

    QPushButton* openCRUDButton = new QPushButton("Open CRUD View", window);
    QPushButton* openReadOnlyButton = new QPushButton("Open Visual View", window);

    QTableWidget* table = new QTableWidget(this);
    QWidget* window_table = new QWidget;
    QPushButton* showTableButton = new QPushButton("Show Table", window);

    void my_window() {
        QVBoxLayout* tablelayout = new QVBoxLayout(window_table);
        table->setColumnCount(5);
        table->setHorizontalHeaderLabels({ "Id", "Title", "Author", "Genre", "Year" });
        tablelayout->addWidget(table);
        window_table->setLayout(tablelayout);
        window_table->setWindowTitle("Book Table");
        window_table->resize(400, 300);

        QVBoxLayout* mainlayout2 = new QVBoxLayout(window);
        QHBoxLayout* mainlayout = new QHBoxLayout(this);

        genreButtonsLayout = new QHBoxLayout();
        mainlayout2->addLayout(genreButtonsLayout);

        this->setLayout(mainlayout);
        this->setWindowTitle("Book list");
        this->resize(700, 250);
        window->setLayout(mainlayout2);
        window->setWindowTitle("Book list - Additional operations");

        QFormLayout* formlayout = new QFormLayout();
        QLabel* title = new QLabel("Title", this);
        QLabel* actor = new QLabel("Author", this);
        QLabel* genre = new QLabel("Genre", this);
        QLabel* year = new QLabel("Year", this);
        QLabel* id = new QLabel("Id", this);

        formlayout->addRow(title, titlebox);
        formlayout->addRow(actor, authorbox);
        formlayout->addRow(genre, genrebox);
        formlayout->addRow(year, yearbox);
        formlayout->addRow(id, idbox);

        QHBoxLayout* buttonlayout = new QHBoxLayout();
        buttonlayout->addWidget(addbutton);
        buttonlayout->addWidget(deletebutton);
        buttonlayout->addWidget(filteryearbutton);

        QLabel* movielistlabel = new QLabel("Additional operations:", window);
        mainlayout2->addWidget(movielistlabel);
        mainlayout2->addWidget(undobutton);
        mainlayout2->addWidget(exitbutton);
        mainlayout2->addWidget(showbutton);
        mainlayout2->addWidget(sortbutton);
        mainlayout2->addWidget(showbasket);
        mainlayout2->addWidget(showTableButton);
        mainlayout2->addWidget(openCRUDButton);
        mainlayout2->addWidget(openReadOnlyButton);

        QVBoxLayout* vlayout = new QVBoxLayout();
        vlayout->addLayout(formlayout);
        vlayout->addLayout(buttonlayout);

        mainlayout->addWidget(booklist);
        mainlayout->addLayout(vlayout);

        window->resize(200, 100);
    }

    void my_basket_window() {
        QHBoxLayout* basketLayout = new QHBoxLayout(basket_window);
        basketLayout->addWidget(bookbasket);

        QFormLayout* basketButtonLayout = new QFormLayout();
        QLabel* basketTitleLabel = new QLabel("Title", this);
        basketButtonLayout->addRow(basketTitleLabel, titlebasketbox);
        basketButtonLayout->addWidget(addbasket);
        basketButtonLayout->addWidget(deletebasket);
        basketButtonLayout->addWidget(generatebasket);
        basketButtonLayout->addWidget(emptybasket);
        basketButtonLayout->addWidget(exportbasket);

        basketLayout->addLayout(basketButtonLayout);
        basket_window->setLayout(basketLayout);
        basket_window->resize(500, 200);
        basket_window->setWindowTitle("Book Basket");
    }

    void genre_buttons() {
        QLayoutItem* item;
        while ((item = genreButtonsLayout->takeAt(0))) {
            delete item->widget();
            delete item;
        }
        genreButtons.clear();

        std::vector<DTOcount> genreCounts = serv->get_genre_counts();
        for (const auto& genreCount : genreCounts) {
            QPushButton* genreButton = new QPushButton(
                QString::fromStdString(genreCount.genre),
                window
            );

            genreButton->setProperty("count", genreCount.count);
            connect(genreButton, &QPushButton::clicked, this, [this, genreButton]() {
                int count = genreButton->property("count").toInt();
                QString genre = genreButton->text();
                QMessageBox::information(
                    this,
                    "Genre Count",
                    QString("%2 books: %1").arg(count).arg(genre)
                );
                });

            genreButtons[genreCount.genre] = genreButton;
            genreButtonsLayout->addWidget(genreButton);
        }
    }

    void refreshBookList() {
        if (!bookModel) {
            bookModel = new BookListModel(serv, booklist);
            booklist->setModel(bookModel);
        }
        bookModel->refresh();
    }

    void refreshBasket() {
        bookbasket->clear();
        for (const auto& book : serv->get_basket_serv()) {
            std::string line = std::to_string(book.get_id()) + "| " + book.get_title() + " by " +
                book.get_author() + ", " + book.get_genre() + ", " +
                std::to_string(book.get_year());
            bookbasket->addItem(QString::fromStdString(line));
        }
    }

    void add_books_from_file() {
        if (!bookModel) {
            bookModel = new BookListModel(serv, booklist);
            booklist->setModel(bookModel);
        }
        bookModel->refresh();
        table_refresh();
    }

    void table_refresh() {
        table->clear();
        table->setColumnCount(5);
        QStringList headers;
        headers << "ID" << "Title" << "Author" << "Genre" << "Year";
        table->setHorizontalHeaderLabels(headers);
        table->setRowCount(serv->get_list_serv().size());

        int row = 0;
        for (const auto& book : serv->get_list_serv()) {
            table->setItem(row, 0, new QTableWidgetItem(QString::number(book.get_id())));
            table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book.get_title())));
            table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(book.get_author())));
            table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(book.get_genre())));
            table->setItem(row, 4, new QTableWidgetItem(QString::number(book.get_year())));
            row++;
        }
        table->resizeColumnsToContents();
    }

    void connect_it() {
        // Selection handling for the list view
        connect(booklist->selectionModel(), &QItemSelectionModel::selectionChanged, [this]() {
            auto indexes = booklist->selectionModel()->selectedIndexes();
            if (indexes.isEmpty()) return;

            QModelIndex index = indexes.first();
            int bookId = index.data(Qt::UserRole).toInt();

            auto it = std::find_if(serv->get_list_serv().begin(), serv->get_list_serv().end(),
                [bookId](const Book& b) { return b.get_id() == bookId; });

            if (it != serv->get_list_serv().end()) {
                const Book& book = *it;
                titlebox->setText(QString::fromStdString(book.get_title()));
                authorbox->setText(QString::fromStdString(book.get_author()));
                genrebox->setText(QString::fromStdString(book.get_genre()));
                yearbox->setText(QString::number(book.get_year()));
                idbox->setText(QString::number(book.get_id()));
            }
            });

        connect(addbutton, &QPushButton::clicked, this, [&]() {
            std::string title = titlebox->text().toStdString();
            std::string author = authorbox->text().toStdString();
            std::string genre = genrebox->text().toStdString();
            int year = yearbox->text().toInt();
            int id = 1000 + rand() % 9000;

            try {
                serv->Add_serv(title, author, genre, year, id);
                refreshBookList();
                genre_buttons();
                titlebox->clear();
                authorbox->clear();
                genrebox->clear();
                yearbox->clear();
                idbox->clear();
                table_refresh();
            }
            catch (const std::exception& e) {
                QMessageBox::warning(this, "Error", e.what());
            }
            });

        connect(deletebutton, &QPushButton::clicked, this, [&]() {
            auto indexes = booklist->selectionModel()->selectedIndexes();
            if (indexes.isEmpty()) {
                QMessageBox::warning(this, "Error", "Select a book first!");
                return;
            }

            QModelIndex index = indexes.first();
            int bookId = index.data(Qt::UserRole).toInt();

            try {
                serv->Delete_serv(bookId);
                refreshBookList();
                genre_buttons();
                titlebox->clear();
                authorbox->clear();
                genrebox->clear();
                yearbox->clear();
                idbox->clear();
                table_refresh();
            }
            catch (const std::exception& e) {
                QMessageBox::warning(this, "Error", e.what());
            }
            });

        connect(undobutton, &QPushButton::clicked, this, [&]() {
            try {
                serv->undo();
                refreshBookList();
                genre_buttons();
                titlebox->clear();
                authorbox->clear();
                genrebox->clear();
                yearbox->clear();
                idbox->clear();
                table_refresh();
            }
            catch (const std::exception& e) {
                QMessageBox::warning(this, "Error", e.what());
            }
            });

        connect(exitbutton, &QPushButton::clicked, this, [&]() {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Exit", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                qApp->exit();
            }
            });

        connect(filteryearbutton, &QPushButton::clicked, this, [&]() {
            int year = yearbox->text().toInt();
            if (year > 0) {
                vector<Book> filtered_books = serv->filter(serv->get_list_serv(), year);
                bookModel->refresh();
            }
            else {
                QMessageBox::warning(this, "Error", "Please provide a valid year");
            }
            titlebox->clear();
            authorbox->clear();
            genrebox->clear();
            yearbox->clear();
            idbox->clear();
            });

        connect(sortbutton, &QPushButton::clicked, this, [&]() {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Sort", "Increasing?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                serv->sort(serv->get_list_serv(), 2, [](const Book& b1, const Book& b2) {
                    return b1.get_title() < b2.get_title();
                    });
            }
            else {
                serv->sort(serv->get_list_serv(), 1, [](const Book& b1, const Book& b2) {
                    return b1.get_title() < b2.get_title();
                    });
            }

            bookModel->refresh();
            table_refresh();
            });

        connect(addbasket, &QPushButton::clicked, this, [&]() {
            std::string title = titlebasketbox->text().toStdString();

            try {
                serv->add_to_basket_serv(title);
                refreshBasket();
            }
            catch (const std::exception& e) {
                QMessageBox::warning(this, "Error", e.what());
            }
            titlebasketbox->clear();
            });

        connect(deletebasket, &QPushButton::clicked, this, [&]() {
            std::string title = titlebasketbox->text().toStdString();
            try {
                serv->delete_basket_serv(title);
                refreshBasket();
            }
            catch (const std::exception& e) {
                QMessageBox::warning(this, "Error", e.what());
            }
            titlebasketbox->clear();
            });

        connect(generatebasket, &QPushButton::clicked, this, [&]() {
            bool ok;
            int size = serv->get_list_serv().size();
            QString input = QInputDialog::getText(
                this,
                "Generate Basket",
                "How many books do you want to generate? (1-" + QString::number(size) + "):",
                QLineEdit::Normal,
                "1",
                &ok
            );

            int n = input.toInt(&ok);

            if (ok) {
                try {
                    serv->generate_basket_serv(n);
                    refreshBasket();
                }
                catch (const std::exception& e) {
                    QMessageBox::warning(this, "Error", e.what());
                }
            }
            });


        connect(emptybasket, &QPushButton::clicked, this, [&]() {
            try {
                serv->delete_whole_basket();
                refreshBasket();
            }
            catch (const std::exception& e) {
                QMessageBox::warning(this, "Error", e.what());
            }
            });

        connect(exportbasket, &QPushButton::clicked, this, [&]() {
            std::string filename = titlebasketbox->text().toStdString();
            try {
                serv->export_basket_serv(filename);
            }
            catch (const std::exception& e) {
                QMessageBox::warning(this, "Error", "The file name must end in .csv or .html");
            }
            });

        connect(showbasket, &QPushButton::clicked, this, [&]() {
            refreshBasket();
            basket_window->show();
            });


        connect(showbutton, &QPushButton::clicked, this, [&]() {
            refreshBookList();
            });

        connect(openCRUDButton, &QPushButton::clicked, this, [&]() {
            auto* crudWindow = new CosCRUDGUI(serv);
            crudWindow->show();
            });

        connect(openReadOnlyButton, &QPushButton::clicked, this, [&]() {
            auto* visualWindow = new CosReadOnlyGUI(serv);
            visualWindow->show();
            });

        connect(showTableButton, &QPushButton::clicked, this, [&]() {
            table_refresh();
            window_table->show();
            });



    }

    void run_tests() {
        test_domain();
        test_repo_interface();
        test_file_repo_specific();
        test_in_memory_repo_specific();
        test_service();
        test_basket();
        test_service_basket_operations();
        test_export_basket_failure_simple();
        test_file_exception_class();
        test_undo_functionality();
        test_load_from_file();
        QMessageBox::information(this, "Tests", "All tests passed!");
    }
};