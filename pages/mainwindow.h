#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pages/frequency_page.h"
//#include "pages/histogram_page.h"
#include "pages/filters_page.h"
#include "pages/harris_page.h"
#include "pages/hough_page.h"
#include "pages/contour_page.h"
#include "pages/matching_page.h"
#include "pages/SIFT_page.h"
#include "pages/thresholding_page.h"
#include "pages/segmenation_page.h"
#include "pages/page11.h"
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
