#include "basket_gui.h"
#include <random>
#include <cstddef> 
#include <vector>
#include <QInputDialog>
#include "service.h"

CosCRUDGUI::CosCRUDGUI(Service* serv) : serv(serv) {
    serv->getBasket().addObserver(this);
    initGUI();
    connectSignals();
    update();
}

CosCRUDGUI::~CosCRUDGUI() {
    serv->getBasket().removeObserver(this);
}

void CosCRUDGUI::initGUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    listWidget = new QListWidget();
    clearButton = new QPushButton("Clear Basket");
    generateButton = new QPushButton("Generate Basket");

    layout->addWidget(listWidget);
    layout->addWidget(clearButton);
    layout->addWidget(generateButton);
}

void CosCRUDGUI::connectSignals() {
    QObject::connect(clearButton, &QPushButton::clicked, [this]() {
        int size = serv->get_list_serv().size();
        if (size > 0) {
            serv->delete_whole_basket();
            update();
        }
       
        });

    QObject::connect(generateButton, &QPushButton::clicked, [this]() {
        bool ok;
        int size = serv->get_list_serv().size();
        int n = QInputDialog::getInt(this, "Generate", "Number of books:", 1, 1, size, 1, &ok);
        if (ok) {
            serv->generate_basket_serv(n);
            update();
        }
        });
}

void CosCRUDGUI::update() {
    listWidget->clear();
    for (const auto& book : serv->get_basket_serv()) {
        QString item = QString::fromStdString(
            std::to_string(book.get_id()) + " | " +
            book.get_title() + " by " +
            book.get_author()
        );
        listWidget->addItem(item);
    }
}




//--------------------------------------------------------------------------



CosReadOnlyGUI::CosReadOnlyGUI(Service* serv) : serv(serv) {
    serv->getBasket().addObserver(this);
    setWindowTitle("Basket Visualization");
    resize(400, 400);
    update();
}

CosReadOnlyGUI::~CosReadOnlyGUI() {
    serv->getBasket().removeObserver(this);
}

void CosReadOnlyGUI::generateRandomPositions() {
    positions.clear();
    colors.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(0, width() - 50);
    std::uniform_int_distribution<> yDist(0, height() - 50);

    for (size_t i = 0; i < serv->get_basket_serv().size(); i++) {
        positions.emplace_back(xDist(gen), yDist(gen));
        colors.emplace_back(
            QColor::fromHsv(rand() % 360, 255, 200) 
        );
    }
}

void CosReadOnlyGUI::paintEvent(QPaintEvent* ev) {
    QPainter p(this);


    for (size_t i = 0; i < positions.size(); i++) {
        p.setBrush(colors[i]);
        p.setPen(Qt::black);
       
        if (i % 3 == 0) {
            p.drawEllipse(positions[i].first, positions[i].second, 40, 40);
        }
        else {
            p.drawRect(positions[i].first, positions[i].second, 40, 40);
        }
       
    }
}

void CosReadOnlyGUI::update() {
    generateRandomPositions();
    repaint();
}