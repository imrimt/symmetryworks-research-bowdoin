#include <QDebug>
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
    int r = a.exec();
    window.close();
    return r;
}
