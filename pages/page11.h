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

    void on_predict_btn_clicked();

    void on_optionsBox_currentIndexChanged(int index);

    void on_n_comp_valueChanged(int arg1);

    void on_comboBox_currentIndexChanged(int index);

    void on_testUploadBtn_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

private:
    void readImages(std::string folderPath, cv::Mat& images, std::vector<std::string>& labels,bool isTest);
    Ui::page11 *ui;
    QPixmap image;
    cv::Mat img;
    QString fileName;
    QString Img;
    std::string filePath;
    bool fileExist=false;
    bool trained=false;
    QString folderPath;
    face_recognition *fr;
    std::unordered_map<std::string, std::string> personToPathMapper;
    std::vector<cv::Mat> predictions;
    std::vector<std::pair<std::vector<float>,std::vector<float>>>ROC;



};

#endif // PAGE11_H
