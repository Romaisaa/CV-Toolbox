#include "pages/mainwindow.h"
#include <QApplication>
#include <QPixmap>
#include <QCursor>
#include <QPoint>
#include "utilities/clickable.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    w.show();

    return a.exec();
}


