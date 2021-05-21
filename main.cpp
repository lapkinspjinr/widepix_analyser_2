#include "widepix_analyser_2.h"
#include <QApplication>
#include <TApplication.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TApplication app("app", nullptr, nullptr);
    widepix_analyser_2 w;

    w.show();
    int result = a.exec();
    return result;
}
