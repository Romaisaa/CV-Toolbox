#include "pages/mainwindow.h"
#include <QApplication>
#include <QPixmap>
#include <QCursor>
#include <QPoint>
#include "clickable.h"

void changeLabelCursors(const QCursor &newCursor)
{
    // Iterate through all widgets in the application
    foreach (QWidget *widget, QApplication::allWidgets())
    {
        qDebug()<<"Here";
        // Check if the widget is a QLabel
        QLabel *label = qobject_cast<QLabel*>(widget);
        if (label != nullptr)
        {
            // Set the new cursor for the QLabel
            label->setCursor(newCursor);
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QPixmap pixmap(":/resources/resources/handpointing.png");
//    QPoint hotSpot(11, 10);
//    QCursor cursor(pixmap, hotSpot.x(), hotSpot.y());
//    changeLabelCursors(cursor);
//    QApplication::setOverrideCursor(cursor);

    MainWindow w;

    w.show();
    a.exec();

    QList<QWidget*> widgets = a.allWidgets();
    qDebug()<<widgets.size();

    return 0;
}


