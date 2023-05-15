#ifndef SIFT_PAGE_H
#define SIFT_PAGE_H

#include <QWidget>
#include <QFileDialog>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <QString>
#include <QLabel>
#include <QImage>
#include <map>
#include "CV/sift.h"
#include "CV/matching.h"
#include <chrono>
namespace Ui {
class page8;
}

class page8 : public QWidget
{
    Q_OBJECT

public:
    explicit page8(QWidget *parent = nullptr);
    ~page8();

private slots:
    void on_image1_clicked();

    void on_image2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::page8 *ui;
    QPixmap image1;
    cv::Mat img1;
    QPixmap image2;
    cv::Mat img2;
    QString fileName1;
    QString fileName2;
    void rescaleImg(QLabel* imgSlot, QPixmap& image);
    void uploadImg(cv::Mat img, QLabel* imgSlot);
    cv::Mat vectorToMat(const vector<vector<float>>& input);
};

#endif // SIFT_PAGE_H
