#include "pages/page6.h"
#include "ui_page6.h"

Page6::Page6(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page6)
{
    ui->setupUi(this);
}

Page6::~Page6()
{
    delete ui;
}

void Page6::on_input_label_clicked()
{
    img_filename = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!img_filename.isEmpty()) {
        cv::Mat Image = cv::imread(img_filename.toStdString());
        Histogram::BGR2Gray(Image,grayImg);
        QImage qGrayImg(grayImg.data, grayImg.cols, grayImg.rows, QImage::Format_Grayscale8);
        QPixmap image  = QPixmap::fromImage(qGrayImg);
        int w = ui->input_label->width();
        int h = ui->input_label->height();
        ui->input_label->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));
        ui->input_label->setScaledContents(true);
        updateOutput();
    }
}

void Page6::updateOutput(){
    if (!img_filename.isEmpty()) {
    cv::Mat outputImage;
    Harris::harrisCornerDetector(grayImg,outputImage,ui->threshold->value(),ui->ksize->value());
    QImage qoutputImage(outputImage.data, outputImage.cols, outputImage.rows, QImage::Format_BGR888);
    QPixmap image  = QPixmap::fromImage(qoutputImage);
    int w = ui->output_label->width();
    int h = ui->output_label->height();
    ui->output_label->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));
    ui->output_label->setScaledContents(true);
    }
}




void Page6::on_ksize_editingFinished()
{
    int val = ui->ksize->value();
        if(val % 2 == 0)
        ui->ksize->setValue(val-1);
}


void Page6::on_threshold_valueChanged(double arg1)
{
    updateOutput();

}




void Page6::on_ksize_valueChanged(int arg1)
{
    int val = ui->ksize->value();
        if(val % 2 == 0)
        ui->ksize->setValue(val-1);
    updateOutput();

}

