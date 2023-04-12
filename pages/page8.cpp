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
        img1 = cv::imread(fileName1.toStdString(), cv::IMREAD_GRAYSCALE);
        QImage qimage(img1.data, img1.cols, img1.rows, QImage::Format_Grayscale8);
        image1 = QPixmap::fromImage(qimage);
        rescaleImg(ui->image1, image1);
        ui->image1->setPixmap(image1);
    }
}


void page8::on_image2_clicked()
{
    fileName2 = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!fileName2.isEmpty()) {

        img2 = cv::imread(fileName2.toStdString(), cv::IMREAD_GRAYSCALE);
        QImage qimage(img2.data, img2.cols, img2.rows, QImage::Format_Grayscale8);
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

void page8::uploadImg(cv::Mat img)
{
    QImage qimage2(img.data, img.cols, img.rows,QImage::Format_Grayscale8);
    QPixmap outputPix = QPixmap::fromImage(qimage2);

    rescaleImg(ui->image3, outputPix);
    ui->image3->setPixmap(outputPix);
}
