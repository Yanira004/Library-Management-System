#include "lab_qt.h"
#include <QtWidgets/QApplication>
#include "gui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Repo* repo = new FileRepo("books.txt");
    Service* serv = new Service(repo);
    GUI window(serv);

    window.show();
    return a.exec();
}
