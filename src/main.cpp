#include <QApplication>
#include "mainwindow.h"
#include "app_context.h"
#include "theme_manager.h"
#include <QCoreApplication>

int main(int argc, char* argv[]) 
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("sensorhub");
    QCoreApplication::setApplicationName("sensorhub-monitor");

    sensorhub::AppContext appCtx;
    appCtx.theme()->loadPersisted();

    MainWindow w;
    w.setAppContext(&appCtx);
    w.setWindowTitle("sensorhub monitor");
    w.resize(800, 500);
    w.show();
    return app.exec();
}