#include "widepix_analyser_2.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    widepix_analyser_2 w;
    w.show();

    return a.exec();
}
