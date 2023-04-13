#ifndef PAGE8_H
#define PAGE8_H

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
//    std::vector<std::vector<cv::Mat>> keypoints1;
//    std::vector<std::vector<cv::Mat>>scale_space1;
//    std::vector<std::vector<float>> descriptors1;
//    std::vector<std::vector<cv::Mat>> keypoints2;
//    std::vector<std::vector<cv::Mat>>scale_space2;
//    std::vector<std::vector<float>> descriptors2;
    cv::Mat vectorToMat(const vector<vector<float>>& input);
};

#endif // PAGE8_H
