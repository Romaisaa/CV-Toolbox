#include "page11.h"
#include "ui_page11.h"

page11::page11(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page11)
{
    ui->setupUi(this);
}

page11::~page11()
{
    delete ui;
}

void page11::on_inputImageLabel_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!fileName.isEmpty()) {
        // read colored image
        img = cv::imread(fileName.toStdString());
        QImage qimage(img.data, img.cols, img.rows, QImage::Format_BGR888);
        image  = QPixmap::fromImage(qimage);
        int w = ui->inputImageLabel->width();
        int h = ui->inputImageLabel->height();
        ui->inputImageLabel->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));
        ui->inputImageLabel->setScaledContents(true);
    }
}

void page11::on_applyButton_clicked()
{
    cv::Mat output(img.size(), img.type());

    if(ui->optionsBox->currentIndex() == 0){
        faces_detection::detect_faces(img, output, false);
    }else{

    }

    QImage qimage(output.data, output.cols, output.rows, QImage::Format_BGR888);
    QPixmap Output  = QPixmap::fromImage(qimage);
    int w = ui->outputImageLabel->width();
    int h = ui->outputImageLabel->height();
    ui->outputImageLabel->setPixmap(Output.scaled(w,h,Qt::KeepAspectRatio));
    ui->outputImageLabel->setScaledContents(true);

}

void readImages(std::string folderPath, cv::Mat& images, std::vector<std::string>& labels) {
    int row = 0, col = 0;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (fs::is_regular_file(entry.path())) {
            row++;
        }
        if (row == 1) {
            cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_GRAYSCALE);
            img = img.reshape(1, 1);
            col = img.size[1];
        }
    }

    images = cv::Mat(row, col, CV_8U);
    int idx = 0;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (fs::is_regular_file(entry.path())) {
            cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_GRAYSCALE);
            img = img.reshape(1, 1);
            cv::Mat row = images.row(idx);
            img.copyTo(row);
            idx++;

            std::vector<std::string> splitParts;

            std::istringstream iss(entry.path().filename().string());
            std::string part;

            while (std::getline(iss, part, '_')) {
                splitParts.push_back(part);
            }

            if (!splitParts.empty()) {
                std::string firstPart = splitParts[0];
                labels.push_back(firstPart);
            }
        }
    }
    images.convertTo(images, CV_32F);
}

void page11::readImages(std::string folderPath, cv::Mat& images, std::vector<std::string>& labels) {
    int row = 0, col = 0;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (fs::is_regular_file(entry.path())) {
            row++;
        }
        if (row == 1) {
            cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_GRAYSCALE);
            img = img.reshape(1, 1);
            col = img.size[1];
        }
    }

    images = cv::Mat(row, col, CV_8U);
    int idx = 0;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (fs::is_regular_file(entry.path())) {
            cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_GRAYSCALE);
            img = img.reshape(1, 1);
            cv::Mat row = images.row(idx);
            img.copyTo(row);
            idx++;

            std::vector<std::string> splitParts;

            std::istringstream iss(entry.path().filename().string());
            std::string part;

            while (std::getline(iss, part, '_')) {
                splitParts.push_back(part);
            }

            if (!splitParts.empty()) {
                std::string firstPart = splitParts[0];
                labels.push_back(firstPart);
            }
        }
    }
    images.convertTo(images, CV_32F);
}

