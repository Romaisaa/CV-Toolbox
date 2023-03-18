#ifndef PAGE1_H
#define PAGE1_H

#include <QWidget>
#include <QFileDialog>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <QString>
#include <QLabel>
#include <QImage>
#include <map>

namespace Ui {
class page1;
}

class page1 : public QWidget
{
    Q_OBJECT

public:
    explicit page1(QWidget *parent = nullptr);
    ~page1();

private slots:
    void on_image_clicked();

    void on_optionCombo_currentIndexChanged(int index);

    void on_intSpinner_valueChanged(int arg1);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_intspin3_2_valueChanged(int arg1);

    void on_intspin3_1_valueChanged(int arg1);

    void on_operationBox_currentIndexChanged(int index);

    void on_intspin5_1_valueChanged(int arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

    void on_intspin5_2_valueChanged(int arg1);

    void on_intspin5_3_valueChanged(int arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_intspin4_1_valueChanged(int arg1);

    void on_dirCombo_currentIndexChanged(int index);

    void on_intspin6_1_valueChanged(int arg1);

private:
    QPixmap imageGray;
    QPixmap image;
    cv::Mat img;
    cv::Mat imgGray;
    QString fileName;
    Ui::page1 *ui;
    void updateOutput();
    void rescaleImg(QLabel* imgSlot, QPixmap& image);
    void uploadImg(cv::Mat img, QImage::Format format);

};

#endif // PAGE1_H
