#ifndef PAGE7_H
#define PAGE7_H

#include <QWidget>
#include <QFileDialog>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <QString>
#include <QLabel>
#include <QImage>
#include <map>
#include "CV/matching.h"

namespace Ui {
class page7;
}

class page7 : public QWidget
{
    Q_OBJECT

public:
    explicit page7(QWidget *parent = nullptr);
    ~page7();

private slots:
    void on_image1_clicked();
    void on_image2_clicked();

    void on_pushButton_clicked();

private:
    Ui::page7 *ui;
    QPixmap image1;
    cv::Mat img1;
    QPixmap image2;
    cv::Mat img2;
    QString fileName1;
    QString fileName2;
    void rescaleImg(QLabel* imgSlot, QPixmap& image);
    void uploadImg(cv::Mat img);
};

#endif // PAGE7_H
