#include "mainwidget.h"
#include <QApplication>
int number;
MainWidget* mw;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.show();
    mw = &w;
    return a.exec();
}
