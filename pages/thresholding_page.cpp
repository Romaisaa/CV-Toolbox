#include "thresholding_page.h"
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
    if(fileName.isEmpty()) return;
    cv::Mat output;
    if(ui->typeCombo->currentIndex() == 0){
        rescaleImg(ui->Image, imageGray);
        ui->Image->setPixmap(imageGray);
        if(ui->methodCombo->currentIndex() == 0){
            Thresholding::optimalThresholding(imgGray, output);
        } else {
            Thresholding::otsuThresholding(imgGray, output);
        }
    }
    else if(ui->typeCombo->currentIndex() == 1){
        rescaleImg(ui->Image, imageGray);
        ui->Image->setPixmap(imageGray);
        Thresholding::localThreshold(imgGray , output, ui->pieceSpin->value());
    }
    else if(ui->typeCombo->currentIndex() == 2){
        rescaleImg(ui->Image, imageGray);
        ui->Image->setPixmap(imageGray);
        Thresholding::multiLevelOtsu(imgGray, output, ui->horizontalSlider->value());
    }
    uploadImg(output, ui->Image2);




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
    QImage qimage2(img.data, img.cols, img.rows,QImage::Format_Grayscale8);
    QPixmap outputPix = QPixmap::fromImage(qimage2);

    rescaleImg(imgSlot, outputPix);
    imgSlot->setPixmap(outputPix);
}


void page9::on_typeCombo_activated(int index)
{
    if(index == 0){
        ui->stackedWidget->setCurrentIndex(0);
        ui->methodCombo->setVisible(true);

    }else if(index == 1){
        ui->stackedWidget->setCurrentIndex(1);
        ui->methodCombo->setVisible(false);

    } else{
        ui->stackedWidget->setCurrentIndex(2);
        ui->methodCombo->setVisible(false);
    }
    updateOutput();

}


void page9::on_horizontalSlider_sliderMoved(int position)
{
    updateOutput();
}


void page9::on_pieceSpin_valueChanged(int arg1)
{
    updateOutput();
}

