#include "phdialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    phDialog w;
    w.show();
    return a.exec();
}
