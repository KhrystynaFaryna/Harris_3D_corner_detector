#include "startingwindow.h"
#include "settingsform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    startingwindow w;

    w.show();

    return a.exec();
}
