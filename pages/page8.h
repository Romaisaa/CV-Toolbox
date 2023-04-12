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

private:
    Ui::page8 *ui;
    QPixmap image1;
    cv::Mat img1;
    QPixmap image2;
    cv::Mat img2;
    QString fileName1;
    QString fileName2;
    void rescaleImg(QLabel* imgSlot, QPixmap& image);
    void uploadImg(cv::Mat img);
};

#endif // PAGE8_H
