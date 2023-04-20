#include "page9.h"
#include "ui_page9.h"

page9::page9(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page9)
{
    ui->setupUi(this);
}

page9::~page9()
{
    delete ui;
}


void page9::on_Image_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!fileName.isEmpty()) {
        // read colored image
        img = cv::imread(fileName.toStdString());
        QImage qimage(img.data, img.cols, img.rows, QImage::Format_BGR888);
        image = QPixmap::fromImage(qimage);

        // read grayscale image
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
        QImage qimageGray(imgGray.data, imgGray.cols, imgGray.rows, QImage::Format_Grayscale8);
        imageGray = QPixmap::fromImage(qimageGray);
    }
    updateOutput();
}

void page9::updateOutput(){
    rescaleImg(ui->Image, image);
    ui->Image->setPixmap(image);
}

void page9::rescaleImg(QLabel* imgSlot, QPixmap& image)
{
    QSize labelSize = imgSlot->size();
    QSize imageSize = image.size();
    if (imageSize.width() > labelSize.width() || imageSize.height() > labelSize.height()) {
        imageSize.scale(labelSize, Qt::KeepAspectRatio);
        image =image.scaled(imageSize, Qt::KeepAspectRatio);
    }
}

void page9::uploadImg(cv::Mat img, QLabel* imgSlot)
{
    QImage qimage2(img.data, img.cols, img.rows,QImage::Format_BGR888);
    QPixmap outputPix = QPixmap::fromImage(qimage2);

    rescaleImg(imgSlot, outputPix);
    imgSlot->setPixmap(outputPix);
}

