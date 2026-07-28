#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Matrix Linear Algebra Toolkit");
    app.setApplicationVersion("2.0 (Qt GUI)");

    // Set a clean application-wide style
    app.setStyle("Fusion");

    MainWindow window;
    window.show();

    return app.exec();
}
