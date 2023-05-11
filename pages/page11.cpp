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

