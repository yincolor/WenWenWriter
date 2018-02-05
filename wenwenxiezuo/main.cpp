#include "mainwindow.h"
#include <QApplication>
#include<QLibrary>
int main(int argc, char *argv[])
{
    QLibrary fcitxlib("libfcitxplatforminputcontextplugin.so");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
