#include "quantizer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Quantizer w;
    w.show();

    return a.exec();
}
