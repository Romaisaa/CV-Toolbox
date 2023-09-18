#include "page4.h"
#include "ui_page4.h"

page4::page4(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page4)
{
    ui->setupUi(this);
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    ui->stackedWidget->setCurrentIndex(0);
    ui->progressBar->setVisible(false);
}

page4::~page4()
{
    delete ui;
}

void page4::on_image1_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!fileName.isEmpty()) {
        // read colored image
        img = cv::imread(fileName.toStdString(), cv::IMREAD_COLOR);
        QImage qimage(img.data, img.cols, img.rows, QImage::Format_BGR888);
        image = QPixmap::fromImage(qimage);
        // using canny with image
        cv::Canny(img, edges, 100, 200);
        rescaleImg(ui->image1, image);
        ui->image1->setPixmap(image);

    }
}

void page4::rescaleImg(QLabel* imgSlot, QPixmap& image)
{
    QSize labelSize = imgSlot->size();
    QSize imageSize = image.size();
    if (imageSize.width() > labelSize.width() || imageSize.height() > labelSize.height()) {
        imageSize.scale(labelSize, Qt::KeepAspectRatio);
        image =image.scaled(imageSize, Qt::KeepAspectRatio);
    }
}



void page4::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0){
        ui->thresholdSpin->setValue(100);
        ui->HoughLineAngleSpin->setValue(1.0);
    }
    else if(index == 1){
        ui->thresholdSpin->setValue(100);
        ui->circleMaxRadiusSpin->setValue(100);
        ui->circleMinRadiusSpin->setValue(10);
        ui->circleRadiusStepSpin->setValue(5);
    }
    else{
        ui->thresholdSpin->setValue(8);
        ui->ellipseMaxRadius->setValue(70);
        ui->ellipseMinRadius->setValue(20);
    }
}


void page4::on_generateBtn_clicked()
{
    cv::Mat output;
    img.copyTo(output);
    ui->progressBar->setVisible(true);
    if(ui->comboBox->currentIndex() == 0){
        std::vector<cv::Vec2d> lines;
        Hough::HoughLine(edges, lines, ui->thresholdSpin->value(), ui->HoughLineAngleSpin->value());
        Hough::drawLines(output, lines, cv::Scalar(0,0,255));

    }else if(ui->comboBox->currentIndex() == 1){
        std::vector<cv::Vec3d> circles;
        Hough::HoughCircle(edges, circles, ui->thresholdSpin->value(), ui->circleMinRadiusSpin->value(), ui->circleMaxRadiusSpin->value(), ui->circleRadiusStepSpin->value(), ui->progressBar);
        Hough::drawCircles(output, circles, cv::Scalar(0,0,255));
    }else if(ui->comboBox->currentIndex() ==2){
        std::vector<cv::Vec6d> ellipses;
        Hough::HoughEllipse(edges, ellipses, ui->thresholdSpin->value(), ui->ellipseMinRadius->value(), ui->ellipseMaxRadius->value(), ui->progressBar);
        Hough::drawEllipse(output, ellipses, cv::Scalar(0,0,255));

    }


    uploadImg(output, ui->image2);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);

}

void page4::uploadImg(cv::Mat img, QLabel* imgSlot)
{
    QImage qimage2(img.data, img.cols, img.rows,QImage::Format_BGR888);
    QPixmap outputPix = QPixmap::fromImage(qimage2);

    rescaleImg(imgSlot, outputPix);
    imgSlot->setPixmap(outputPix);
}


