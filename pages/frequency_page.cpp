#include "frequency_page.h"
#include "ui_page3.h"



page3::page3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page3)
{
    ui->setupUi(this);
}

page3::~page3()
{
    delete ui;
}


void page3::on_low_freq_img_label_clicked()
{
    low_filename = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!low_filename.isEmpty()) {
         low_img = cv::imread(low_filename.toStdString());
        Histogram::BGR2Gray(low_img,low_img);
        QImage qlowImageGray(low_img.data, low_img.cols, low_img.rows, QImage::Format_Grayscale8);
        QPixmap image  = QPixmap::fromImage(qlowImageGray);
        int w = ui->low_freq_img_label->width();
        int h = ui->low_freq_img_label->height();
        ui->low_freq_img_label->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));
        ui->low_freq_img_label->setScaledContents(true);
        updateOutput();

    }
}


void page3::on_high_freq_img_label_clicked()
{
    high_filename = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!high_filename.isEmpty()) {

        high_img = cv::imread(high_filename.toStdString());
        Histogram::BGR2Gray(high_img,high_img);
        QImage qHighImageGray(high_img.data, high_img.cols, high_img.rows, QImage::Format_Grayscale8);
        QPixmap image  = QPixmap::fromImage(qHighImageGray);
        int w = ui->high_freq_img_label->width();
        int h = ui->high_freq_img_label->height();
        ui->high_freq_img_label->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));
        ui->high_freq_img_label->setScaledContents(true);
        updateOutput();

    }
}


void page3::updateOutput()
{
    int low_res=ui->low_res->value();
    int high_res=ui->high_res->value();
    cv::Mat filtered_low;
    cv::Mat filtered_high;
    if (!low_filename.isEmpty()){
        Frequency::applyFilter(low_img,filtered_low,Frequency::LOW,low_res);
        QImage qimage(filtered_low.data, filtered_low.cols, filtered_low.rows,filtered_low.step,QImage::Format_Grayscale8);
        QPixmap* image = new QPixmap(QPixmap::fromImage(qimage));
        int w = ui->low_freq_filtered_img_label->width();
        int h = ui->low_freq_filtered_img_label->height();
        ui->low_freq_filtered_img_label->setPixmap(image->scaled(w,h,Qt::KeepAspectRatio));

    }
    if (!high_filename.isEmpty()){
        Frequency::applyFilter(high_img,filtered_high,Frequency::HIGH,high_res);
        QImage qimage(filtered_high.data, filtered_high.cols, filtered_high.rows,filtered_high.step,QImage::Format_Grayscale8);
        QPixmap* image = new QPixmap(QPixmap::fromImage(qimage));
        int w = ui->high_freq_filtered_img_label->width();
        int h = ui->high_freq_filtered_img_label->height();
        ui->high_freq_filtered_img_label->setPixmap(image->scaled(w,h,Qt::KeepAspectRatio));

    }

    if(!high_filename.isEmpty() && !low_filename.isEmpty())
    {
        cv::Mat hybirdImage;
        Frequency::hybirdImage(filtered_low,filtered_high,hybirdImage);
        QImage qimage(hybirdImage.data, hybirdImage.cols, hybirdImage.rows,hybirdImage.step,QImage::Format_Grayscale8);
        QPixmap* image = new QPixmap(QPixmap::fromImage(qimage));
        int w = ui->output_img_label->width();
        int h = ui->output_img_label->height();
        ui->output_img_label->setPixmap(image->scaled(w,h,Qt::KeepAspectRatio));


    }
}


void page3::on_low_res_valueChanged(int arg1)
{
    updateOutput();
}


void page3::on_high_res_valueChanged(int arg1)
{
    updateOutput();
}

