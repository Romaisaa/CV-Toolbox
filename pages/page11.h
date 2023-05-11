#ifndef PAGE11_H
#define PAGE11_H

#include <QWidget>
#include <QFileDialog>
#include "CV/faces_detection.h"
#include "CV/face_recognition.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
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

    void on_select_folder_btn_clicked();

    void on_train_pca_btn_clicked();

private:
    void readImages(std::string folderPath, cv::Mat& images, std::vector<std::string>& labels);
    Ui::page11 *ui;
    QPixmap image;
    cv::Mat img;
    QString fileName;
    QString Img;
    std::string filePath;
    bool fileExist=false;
    QString folderPath;
     face_recognition *fr;
};

#endif // PAGE11_H
