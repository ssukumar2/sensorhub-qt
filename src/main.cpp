#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[]) 
{
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowTitle("sensorhub monitor");
    w.resize(800, 500);
    w.show();
    return app.exec();
}