#include "page8.h"
#include "ui_page8.h"

page8::page8(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page8)
{
    ui->setupUi(this);
}

page8::~page8()
{
    delete ui;
}

void page8::on_image1_clicked()
{
    fileName1 = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!fileName1.isEmpty()) {
        // read colored image
        img1 = cv::imread(fileName1.toStdString());
        QImage qimage(img1.data, img1.cols, img1.rows, QImage::Format_BGR888);
        image1 = QPixmap::fromImage(qimage);
        rescaleImg(ui->image1, image1);
        ui->image1->setPixmap(image1);
    }
}


void page8::on_image2_clicked()
{
    fileName2 = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!fileName2.isEmpty()) {

        img2 = cv::imread(fileName2.toStdString());
        QImage qimage(img2.data, img2.cols, img2.rows, QImage::Format_BGR888);
        image2 = QPixmap::fromImage(qimage);
        rescaleImg(ui->image2, image2);
        ui->image2->setPixmap(image2);
        cv::imshow("img", img2);
    }
}


void page8::rescaleImg(QLabel* imgSlot, QPixmap& image)
{
    QSize labelSize = imgSlot->size();
    QSize imageSize = image.size();
    if (imageSize.width() > labelSize.width() || imageSize.height() > labelSize.height()) {
        imageSize.scale(labelSize, Qt::KeepAspectRatio);
        image =image.scaled(imageSize, Qt::KeepAspectRatio);
    }
}

void page8::uploadImg(cv::Mat img, QLabel* imgSlot)
{
    QImage qimage2(img.data, img.cols, img.rows,QImage::Format_BGR888);
    QPixmap outputPix = QPixmap::fromImage(qimage2);

    rescaleImg(imgSlot, outputPix);
    imgSlot->setPixmap(outputPix);
}

void page8::on_pushButton_clicked()
{
    if (fileName2.isEmpty()) return;
    cv::Mat keypoints_img;
    Sift::sift_keypoints(img1, keypoints_img, keypoints1, scale_space1,ui->sigmaSpin->value(), ui->ContrastSpin->value(), ui->EdgeSpin->value(), ui->KSpin->value());
    uploadImg(keypoints_img, ui->image1);
}

void page8::on_pushButton_2_clicked()
{
    Sift::SIFT_descriptors(descriptors1, scale_space1, keypoints1, ui->MagSpin->value(), ui->sigmaSpin->value(), ui->KSpin->value());
    cv::Mat keypoints_img;

    Sift::sift_keypoints(img2, keypoints_img, keypoints2, scale_space2,ui->sigmaSpin->value(), ui->ContrastSpin->value(), ui->EdgeSpin->value(), ui->KSpin->value());
    Sift::SIFT_descriptors(descriptors2, scale_space2, keypoints2, ui->MagSpin->value(), ui->sigmaSpin->value(), ui->KSpin->value());

    cv::Mat des1 = vectorToMat(descriptors1);
    cv::Mat des2 = vectorToMat(descriptors2);

    std::vector<cv::Vec3d> matches;
    matching::matchDes(des1, des2, matches);
    cv::Mat output;
    cv::hconcat(img1, img2, output);

    for(int i = 0; i < matches.size(); i++){


    }

}

cv::Mat page8::vectorToMat(const vector<vector<float>>& input) {
    Mat output(input.size(), input[0].size(), CV_32F);

    for (size_t i = 0; i < input.size(); i++) {
        for (size_t j = 0; j < input[i].size(); j++) {
            output.at<float>(i, j) = input[i][j];
        }
    }

    return output;
}
