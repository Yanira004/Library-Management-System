#pragma once
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QImage>
#include <random>
#include "service.h"
#include "observer.h"
#include "domain.h"

class CosCRUDGUI : public QWidget, public Observer {
    Q_OBJECT
private:
    Service* serv;
    QListWidget* listWidget;
    QPushButton* clearButton;
    QPushButton* generateButton;

    void initGUI();
    void connectSignals();
    void update() override;

public:
    CosCRUDGUI(Service* serv);
    ~CosCRUDGUI() override;
};





class CosReadOnlyGUI : public QWidget, public Observer {
    Q_OBJECT
private:
    Service* serv;
    std::vector<std::pair<int, int>> positions;
    std::vector<QColor> colors;

    void update() override;
    void paintEvent(QPaintEvent* ev) override;
    void generateRandomPositions();

public:
    CosReadOnlyGUI(Service* serv);
    ~CosReadOnlyGUI() override;
};