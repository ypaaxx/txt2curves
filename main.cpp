#include "txt2curves.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    txt2curves w;
    w.show();

    return a.exec();
}
