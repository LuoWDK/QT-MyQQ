#include "qq_widget.h"
#include "qq_drawer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();

    Drawer drawer;
    drawer.resize(250, 700);
    drawer.show();

    return a.exec();
}
