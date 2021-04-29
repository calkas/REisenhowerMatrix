#include "reisenhowermatrixmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    REisenhowerMatrixMainWindow w;
    w.show();
    return a.exec();
}
