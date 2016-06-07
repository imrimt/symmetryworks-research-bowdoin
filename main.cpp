#include <QDebug>
#include <QApplication>

#include "interface.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    interface app_interface;
    app_interface.show();


    return a.exec();
}
