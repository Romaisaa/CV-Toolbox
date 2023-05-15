#include "histogram_page.h"
#include "ui_page2.h"

page2::page2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page2)
{
    ui->setupUi(this);
    ui->moreGraphsStack->setCurrentIndex(1);
    ui->stackedWidget->setCurrentIndex(0);
    ui->normalizeStack->setCurrentIndex(0);
    ui->ModeCombo1->setVisible(false);


}

page2::~page2()
{
    delete ui;
}

void page2::uploadImage()
{

    fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!fileName.isEmpty()) {
        // read colored image
        img = cv::imread(fileName.toStdString());
        QImage qimage(img.data, img.cols, img.rows, QImage::Format_BGR888);
        image = QPixmap::fromImage(qimage);

        // read grayscale image
        Histogram::BGR2Gray(img,imgGray);
        QImage qimageGray(imgGray.data, imgGray.cols, imgGray.rows, QImage::Format_Grayscale8);
        imageGray = QPixmap::fromImage(qimageGray);



    }
    page2::updateOutput();





}



void page2::on_ModeCombo1_currentIndexChanged(int index)
{
    ui->normalizeStack->setCurrentIndex(index);
    if(index==1)
        normalizeMode=true;
    else
        normalizeMode=false;
    page2::updateOutput();
}

void page2::updateOutput()
{
    if(fileName.isEmpty()) return;
    if(ui->OperationBox->currentIndex()==0){

        cv::Mat output;
        Histogram::BGR2Gray(img, output);

        QImage qimage2(output.data, output.cols, output.rows, QImage::Format_Grayscale8);
        QPixmap outputPix = QPixmap::fromImage(qimage2);

        rescaleImg(ui->image4,outputPix);
        rescaleImg(ui->image3,image);

        ui->image3->setPixmap(image);
        ui->image4->setPixmap(outputPix);
        cv::Mat blue, green, red;
        Histogram::getChannels(img, blue, green, red);

        QVector<double> hist1, dist1, hist2,dist2, hist3,dist3;
        cv::Mat pdf1 = cv::Mat::zeros(256, 1, CV_32F);
        cv::Mat cdf1 = cv::Mat::zeros(256, 1, CV_32F);
        cv::Mat pdf2 = cv::Mat::zeros(256, 1, CV_32F);
        cv::Mat cdf2 = cv::Mat::zeros(256, 1, CV_32F);
        cv::Mat pdf3 = cv::Mat::zeros(256, 1, CV_32F);
        cv::Mat cdf3 = cv::Mat::zeros(256, 1, CV_32F);
        Histogram::getHist(red, pdf1, cdf1);
        Histogram::getHist(green, pdf2, cdf2);
        Histogram::getHist(blue, pdf3, cdf3);

        for(int i = 0; i< 256; i++){
           hist1.append(pdf1.at<float>(i));
           dist1.append(cdf1.at<float>(i));
           hist2.append(pdf2.at<float>(i));
           dist2.append(cdf2.at<float>(i));
           hist3.append(pdf3.at<float>(i));
           dist3.append(cdf3.at<float>(i));
        }

        plotter::plotHist(ui->histPlot1, hist1, Qt::red);
        plotter::plotHist(ui->distPlot1, dist1, Qt::red);
        plotter::plotHist(ui->histPlot2, hist2, Qt::green);
        plotter::plotHist(ui->distPlot2, dist2, Qt::green);
        plotter::plotHist(ui->histPlot3, hist3, Qt::blue);
        plotter::plotHist(ui->distPlot3, dist3, Qt::blue);

    }
    if(ui->OperationBox->currentIndex()==1){
        ui->moreGraphsStack->setCurrentIndex(0);
       cv::Mat output;
       if(normalizeMode)
           Histogram::normalizeImg(imgGray, output, ui->minNormVal->value(), ui->maxNormVal->value());
       else
           Histogram::histEqualizer(imgGray,output);

       QImage qimage2(output.data, output.cols, output.rows, QImage::Format_Grayscale8);
       QPixmap outputPix = QPixmap::fromImage(qimage2);

       rescaleImg(ui->image2, outputPix);
       rescaleImg(ui->image,imageGray);

       QVector<double> hist1, dist1, hist2,dist2;
       cv::Mat pdf1 = cv::Mat::zeros(256, 1, CV_32F);
       cv::Mat cdf1 = cv::Mat::zeros(256, 1, CV_32F);
       cv::Mat pdf2 = cv::Mat::zeros(256, 1, CV_32F);
       cv::Mat cdf2 = cv::Mat::zeros(256, 1, CV_32F);
       Histogram::getHist(imgGray, pdf1, cdf1);
       Histogram::getHist(output, pdf2, cdf2);

       for(int i = 0; i< 256; i++){
          hist1.append(pdf1.at<float>(i));
          dist1.append(cdf1.at<float>(i));
          hist2.append(pdf2.at<float>(i));
          dist2.append(cdf2.at<float>(i));
       }



       ui->image->setPixmap(imageGray);
       ui->image2->setPixmap(outputPix);
       plotter::plotHist(ui->histPlot1, hist1, Qt::red);
       plotter::plotHist(ui->distPlot1, dist1, Qt::blue);
       plotter::plotHist(ui->histPlot2, hist2, Qt::red);
       plotter::plotHist(ui->distPlot2, dist2, Qt::blue);

    }

if(ui->OperationBox->currentIndex()==2){

    rescaleImg(ui->image5,imageGray);
    ui->image5->setPixmap(imageGray);
if(ui->thres_combox->currentIndex()==0){
    cv::Mat output;
    Histogram::globalThreshold(imgGray,output);
    QImage qimage2(output.data, output.cols, output.rows, QImage::Format_Grayscale8);
    QPixmap outputPix = QPixmap::fromImage(qimage2);
    rescaleImg(ui->image6, outputPix);
    ui->image6->setPixmap(outputPix);
}else{
    cv::Mat output;
    Histogram::localThreshold(imgGray,output,ui->pieceSize_spiner->value());
    QImage qimage2(output.data, output.cols, output.rows, QImage::Format_Grayscale8);
    QPixmap outputPix = QPixmap::fromImage(qimage2);
    rescaleImg(ui->image6, outputPix);
    ui->image6->setPixmap(outputPix);
    }
}







}

void page2::rescaleImg(QLabel* imgSlot, QPixmap& image)
{
    QSize labelSize = imgSlot->size();
    QSize imageSize = image.size();
    if (imageSize.width() > labelSize.width() || imageSize.height() > labelSize.height()) {
        imageSize.scale(labelSize, Qt::KeepAspectRatio);
        image =image.scaled(imageSize, Qt::KeepAspectRatio);
       }
}


void page2::on_minNormVal_valueChanged(int arg1)
{
    updateOutput();
}


void page2::on_maxNormVal_valueChanged(int arg1)
{
    updateOutput();
}


void page2::on_OperationBox_currentIndexChanged(int index)
{
    if(index==0){
        ui->moreGraphsStack->setCurrentIndex(1);
        ui->stackedWidget->setCurrentIndex(0);
        ui->normalizeStack->setCurrentIndex(ui->ModeCombo1->currentIndex());
        ui->ModeCombo1->setVisible(false);
    }
    else if(index==1){
        ui->moreGraphsStack->setCurrentIndex(0);
        ui->stackedWidget->setCurrentIndex(0);
        ui->normalizeStack->setCurrentIndex(0);
        ui->ModeCombo1->setVisible(true);
    }else if(index==2){
        ui->moreGraphsStack->setCurrentIndex(2);
        ui->stackedWidget->setCurrentIndex(1);
        ui->normalizeStack->setCurrentIndex(2);
        ui->ModeCombo1->setVisible(false);
    }
    updateOutput();
}


void page2::on_thres_combox_currentIndexChanged(int index)
{
    if(index==0){
        ui->pieceSize_spiner->setDisabled(true);
    }
    else{
        ui->pieceSize_spiner->setDisabled(false);
    }
    updateOutput();
}


void page2::on_pieceSize_spiner_valueChanged(int arg1)
{
    updateOutput();
}


void page2::on_image5_clicked()
{
    uploadImage();
}


void page2::on_image3_clicked()
{
    uploadImage();
}


void page2::on_image_clicked()
{
    uploadImage();
}

