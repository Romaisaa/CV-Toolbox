#include "page10.h"
#include "ui_page10.h"

Page10::Page10(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page10)
{
    ui->setupUi(this);
}

Page10::~Page10()
{
    delete ui;
}

void Page10::on_input_image_label_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!fileName.isEmpty()) {
        // read colored image
        img = cv::imread(fileName.toStdString());
        QImage qimage(img.data, img.cols, img.rows, QImage::Format_BGR888);
        image  = QPixmap::fromImage(qimage);
        int w = ui->input_image_label->width();
        int h = ui->input_image_label->height();
        ui->input_image_label->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));
        ui->input_image_label->setScaledContents(true);
    }
}


void Page10::on_applyBtn_clicked()
{
    cv::Mat output;
    Segmentation::meanShiftSegmentation(img,output,ui->doubleSpinBox_1->value(),ui->doubleSpinBox_2->value(),ui->spinBox_1->value());
    QImage qimage(output.data, output.cols, output.rows, QImage::Format_BGR888);
    QPixmap Output  = QPixmap::fromImage(qimage);
    int w = ui->output_image_label->width();
    int h = ui->output_image_label->height();
    ui->output_image_label->setPixmap(Output.scaled(w,h,Qt::KeepAspectRatio));
    ui->output_image_label->setScaledContents(true);

}

