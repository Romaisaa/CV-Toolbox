#ifndef PAGE11_H
#define PAGE11_H

#include <QWidget>
#include <QFileDialog>
#include "CV/faces_detection.h"

namespace Ui {
class page11;
}

class page11 : public QWidget
{
    Q_OBJECT

public:
    explicit page11(QWidget *parent = nullptr);
    ~page11();

private slots:
    void on_inputImageLabel_clicked();


    void on_applyButton_clicked();

private:
    void readImages(std::string folderPath, cv::Mat& images, std::vector<std::string>& labels);
    Ui::page11 *ui;
    QPixmap image;
    cv::Mat img;
    QString fileName;
    QString Img;
};

#endif // PAGE11_H
