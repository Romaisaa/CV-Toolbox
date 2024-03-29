#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile f(":/resources/style/Cstartpage.qss");
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
    this->setWindowTitle("CV ToolKit");

    ui->stackedWidget->insertWidget(0,new page1());
    ui->stackedWidget->insertWidget(1,new page2());
    ui->stackedWidget->insertWidget(2,new page3());
    ui->stackedWidget->insertWidget(3,new page4());
    ui->stackedWidget->insertWidget(4,new page5());
    ui->stackedWidget->insertWidget(5,new Page6());
    ui->stackedWidget->insertWidget(6,new page7());
    ui->stackedWidget->insertWidget(7,new page8());
    ui->stackedWidget->insertWidget(8,new page9());
    ui->stackedWidget->insertWidget(9,new Page10());
    ui->stackedWidget->insertWidget(10,new page11());

    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->pushButton, &QPushButton::clicked, ui->stackedWidget, [=]() {
            ui->stackedWidget->setCurrentIndex(0);
        });
    connect(ui->pushButton_2, &QPushButton::clicked, ui->stackedWidget, [=]() {
            ui->stackedWidget->setCurrentIndex(1);
        });
    connect(ui->pushButton_3, &QPushButton::clicked, ui->stackedWidget, [=]() {
            ui->stackedWidget->setCurrentIndex(2);
        });
    connect(ui->pushButton_4, &QPushButton::clicked, ui->stackedWidget, [=]() {
            ui->stackedWidget->setCurrentIndex(3);
        });
    connect(ui->pushButton_5, &QPushButton::clicked, ui->stackedWidget, [=]() {
            ui->stackedWidget->setCurrentIndex(4);
        });
    connect(ui->pushButton_6, &QPushButton::clicked, ui->stackedWidget, [=]() {
            ui->stackedWidget->setCurrentIndex(5);
        });
    connect(ui->pushButton_7, &QPushButton::clicked, ui->stackedWidget, [=]() {
            ui->stackedWidget->setCurrentIndex(6);
        });
    connect(ui->pushButton_8, &QPushButton::clicked, ui->stackedWidget, [=]() {
            ui->stackedWidget->setCurrentIndex(7);
        });
    connect(ui->pushButton_9, &QPushButton::clicked, ui->stackedWidget, [=]() {
            ui->stackedWidget->setCurrentIndex(8);
        });
    connect(ui->pushButton_10, &QPushButton::clicked, ui->stackedWidget, [=]() {
        ui->stackedWidget->setCurrentIndex(9);
    });
    connect(ui->pushButton_11, &QPushButton::clicked, ui->stackedWidget, [=]() {
        ui->stackedWidget->setCurrentIndex(10);
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
            // Set the new curso2r for the QLabel
            label->setCursor(pointerCursor);
            continue;
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
