#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pages/page3.h"
//#include "pages/page2.h"
#include "pages/page1.h"
#include "pages/page6.h"
#include "pages/page4.h"
#include "pages/page5.h"
#include "pages/page7.h"
#include "pages/page8.h"
#include "utilities/clickable.h"
#include <QFile>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
