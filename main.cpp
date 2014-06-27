#include "mainwindow.h"
#include "displayableobject.h"
#include <QApplication>
#include "line.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
