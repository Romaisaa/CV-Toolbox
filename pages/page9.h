#ifndef PAGE9_H
#define PAGE9_H

#include <QWidget>
#include <QFileDialog>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <QString>
#include <QLabel>
#include <QImage>
#include <map>
#include "CV/thresholding.h"

namespace Ui {
class page9;
}

class page9 : public QWidget
{
    Q_OBJECT

public:
    explicit page9(QWidget *parent = nullptr);
    ~page9();

private slots:

    void on_Image_clicked();

    void on_typeCombo_activated(int index);

    void on_horizontalSlider_sliderMoved(int position);

    void on_pieceSpin_valueChanged(int arg1);

private:
    Ui::page9 *ui;
    QPixmap image;
    cv::Mat img;
    QPixmap imageGray;
    cv::Mat imgGray;
    QString fileName;
    void rescaleImg(QLabel* imgSlot, QPixmap& image);
    void uploadImg(cv::Mat img, QLabel* imgSlot);
    void updateOutput();
};

#endif // PAGE9_H
