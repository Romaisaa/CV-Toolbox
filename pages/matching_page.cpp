#include "matching_page.h"
#include "ui_page7.h"

page7::page7(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page7)
{
    ui->setupUi(this);
}

page7::~page7()
{
    delete ui;
}

void page7::on_image1_clicked()
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



void page7::rescaleImg(QLabel* imgSlot, QPixmap& image)
{
    QSize labelSize = imgSlot->size();
    QSize imageSize = image.size();
    if (imageSize.width() > labelSize.width() || imageSize.height() > labelSize.height()) {
        imageSize.scale(labelSize, Qt::KeepAspectRatio);
        image =image.scaled(imageSize, Qt::KeepAspectRatio);
    }
}

void page7::uploadImg(cv::Mat img)
{
    QImage qimage2(img.data, img.cols, img.rows,QImage::Format_Grayscale8);
    QPixmap outputPix = QPixmap::fromImage(qimage2);

    rescaleImg(ui->image3, outputPix);
    ui->image3->setPixmap(outputPix);
}

void page7::on_image2_clicked()
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


void page7::on_pushButton_clicked()
{

    if(fileName1.isEmpty() || fileName2.isEmpty()) return ;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    cv::Rect roi;

    if(ui->comboBox->currentIndex()== 1){

        matching::crossCorrelation(img1, img2, roi);

    } else if(ui->comboBox->currentIndex()== 0) {

        matching::SSD(img1, img2, roi);

    }

    cv::Mat outImg = img1.clone();
    cv::rectangle(outImg, roi, cv::Scalar(0, 255, 0), 2);
    uploadImg(outImg);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::string time="Time Elapsed:  "+std::to_string(std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count()) + "  µs" ;

    ui->time_label->setText( QString::fromStdString(time));
}

