#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(950, 920);
    w.setWindowTitle("Qt Microfluidic Chip Simulation");
    w.show();

    return a.exec();
}
