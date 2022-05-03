#include <QtWidgets/QApplication>

#include "MainWindow.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // qDebug() << QT_VERSION_STR;
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}
