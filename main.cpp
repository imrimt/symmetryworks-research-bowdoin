#include <QDebug>
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    interface app_interface;
    MainWindow window(&app_interface);

    QHBoxLayout *centerLayout = new QHBoxLayout();
    QWidget *centerWidget = new QWidget();
    centerLayout->addWidget(&app_interface);
    centerWidget->setLayout(centerLayout);

    window.setCentralWidget(centerWidget);
    window.resize(app_interface.width() * 1.05, app_interface.height() * 1.05);
    window.show();
    // app_interface.show();

    return a.exec();
}
