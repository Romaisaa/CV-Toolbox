#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString fileName = "Cstartpage.qss";
    QString projectDirPath =__FILE__;
    projectDirPath+="/../../style";
    QString filePath = QDir(projectDirPath).absoluteFilePath(fileName);
    QFile f(filePath);
    if ( !f.exists() )
    {
       qWarning() << "Unable to set dark stylesheet, file not found";
    }
    else
    {
       f.open( QFile::ReadOnly | QFile::Text );
       QTextStream ts( &f );
       this->setStyleSheet( ts.readAll() );
    }
    QPixmap pixmap(":/resources/resources/default.png");
    QPoint hotSpot(11, 10);
    QCursor cursor(pixmap, hotSpot.x(), hotSpot.y());
    this->setCursor(cursor);


    QButtonGroup *group = new QButtonGroup();
    group->addButton(ui->pushButton);
    group->addButton(ui->pushButton2);
    group->addButton(ui->pushButton_3);
    group->addButton(ui->pushButton_4);


    ui->stackedWidget->insertWidget(0,new page1());
//    ui->stackedWidget->insertWidget(1,new page2());
    ui->stackedWidget->insertWidget(2,new page3());
    ui->stackedWidget->insertWidget(3,new page4());
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->pushButton, &QPushButton::clicked, ui->stackedWidget, [=]() {
            ui->stackedWidget->setCurrentIndex(0);
        });
//    connect(ui->pushButton2, &QPushButton::clicked, ui->stackedWidget, [=]() {
//            ui->stackedWidget->setCurrentIndex(1);
//        });
    connect(ui->pushButton_3, &QPushButton::clicked, ui->stackedWidget, [=]() {
            ui->stackedWidget->setCurrentIndex(2);
        });
    connect(ui->pushButton_4, &QPushButton::clicked, ui->stackedWidget, [=]() {
            ui->stackedWidget->setCurrentIndex(3);
        });


    QPixmap pointPix(":/resources/resources/handpointing.png");
    QPoint hotSpot2(11, 10);
    QCursor pointerCursor(pointPix, hotSpot2.x(), hotSpot2.y());
    QList<QWidget *> widgets= this->findChildren<QWidget *>();
    foreach (QWidget *widget, widgets)
    {
        // Check if the widget is a QLabel
        Clickable *label = qobject_cast<Clickable*>(widget);
        if (label != nullptr)
        {
            // Set the new cursor for the QLabel
            label->setCursor(pointerCursor);
            continue;
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


