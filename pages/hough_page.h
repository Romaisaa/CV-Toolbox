#ifndef HOUGH_PAGE_H
#define HOUGH_PAGE_H

#include <QWidget>
#include <QFileDialog>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <QString>
#include <QLabel>
#include <QImage>
#include <CV/hough.h>

namespace Ui {
class page4;
}

class page4 : public QWidget
{
    Q_OBJECT

public:
    explicit page4(QWidget *parent = nullptr);
    ~page4();

private slots:
    void on_image1_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_generateBtn_clicked();

private:
    QPixmap image;
    cv::Mat img;
    cv::Mat edges;
    QString fileName;
    void rescaleImg(QLabel* imgSlot, QPixmap& image);
    void uploadImg(cv::Mat img, QLabel* imgSlot);
    Ui::page4 *ui;
};

#endif // HOUGH_PAGE_H
