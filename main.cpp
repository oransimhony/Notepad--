#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle(QObject::tr("++Notepad"));
    w.setWindowIcon(QIcon("./images/favicon.ico"));
    w.show();

    return a.exec();
}
