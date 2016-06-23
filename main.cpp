#include <QDebug>
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    // interface app_interface;
    MainWindow window;

    window.show();
    // app_interface.show();

    return a.exec();
}
