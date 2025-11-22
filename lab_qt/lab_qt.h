#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_lab_qt.h"

class lab_qt : public QMainWindow
{
    Q_OBJECT

public:
    lab_qt(QWidget *parent = nullptr);
    ~lab_qt();

private:
    Ui::lab_qtClass ui;
};
