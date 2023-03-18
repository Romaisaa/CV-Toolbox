#include "page1.h"
#include "ui_page1.h"
#include "CV/filters.h"

page1::page1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page1)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->label1_1->setText("Noise Value");
}

page1::~page1()
{
    delete ui;
}

void page1::updateOutput()
{
  if(fileName.isEmpty()) return ;

  if(ui->operationBox->currentIndex()== 0){
      rescaleImg(ui->image, image);
      ui->image->setPixmap(image);
      if(ui->optionCombo->currentIndex()==0){
          cv::Mat output(img.rows, img.cols, img.type());

          filters::add_uniform_noise(img, output, ui->intSpinner->value());
          uploadImg(output, QImage::Format_BGR888);

      }else if(ui->optionCombo->currentIndex()==1) {
          cv::Mat output(img.rows, img.cols, img.type());
          filters::add_gaussian_noise(img, output,ui->intspin3_2->value(), ui->intspin3_2->value());
          uploadImg(output, QImage::Format_BGR888);
      }else if(ui->optionCombo->currentIndex()==2) {
          cv::Mat output(img.rows, img.cols, img.type());
          filters::add_salt_and_pepper_noise(img, output,ui->doubleSpinBox->value());
          uploadImg(output, QImage::Format_BGR888);
      }
  }else if(ui->operationBox->currentIndex()== 1){
      rescaleImg(ui->image, imageGray);
      ui->image->setPixmap(imageGray);
      cv::Mat output(imgGray.rows, imgGray.cols, imgGray.type());

      if(ui->optionCombo->currentIndex()==0){
          std::map<int, filters::SobelType>filterType;
          filterType[0] =filters::X;
          filterType[1] = filters::Y;
          filterType[2] = filters::COMBINED;
          filters::sobelEdgeDetector(imgGray, output, ui->intspin6_1->value(), filterType[ui->dirCombo->currentIndex()]);
      }else if(ui->optionCombo->currentIndex()==1){
          filters::prewitt_robertEdgeDetectors(imgGray, output, ui->intSpinner->value(), filters::ROBERT);
      }else if(ui->optionCombo->currentIndex()==2){
          filters::cannyEdgeDetector(imgGray, output, ui->intspin5_2->value(),ui->intspin5_3->value(),ui->intspin5_1->value(),ui->doubleSpinBox_3->value());
      }else if(ui->optionCombo->currentIndex()==3){
          filters::prewitt_robertEdgeDetectors(imgGray, output, ui->intSpinner->value(), filters::PERWITT);
      }
      uploadImg(output, QImage::Format_Grayscale8);

  }else if(ui->operationBox->currentIndex() == 2) {
      rescaleImg(ui->image, imageGray);
      ui->image->setPixmap(imageGray);
      cv::Mat output(imgGray.rows, imgGray.cols, imgGray.type());
      if(ui->optionCombo->currentIndex()==0){
          filters::apply_average_filter(imgGray, output, ui->intSpinner->value());
      }else if(ui->optionCombo->currentIndex()==1){
          filters::apply_median_filter(imgGray, output, ui->intSpinner->value());
      }else if(ui->optionCombo->currentIndex()==2){
          // To be changed
          filters::apply_gaussian_filter(imgGray, output, ui->intspin4_1->value(), ui->doubleSpinBox_2->value());
      }
      uploadImg(output, QImage::Format_Grayscale8);
  }
}

void page1::on_image_clicked()
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

void page1::rescaleImg(QLabel* imgSlot, QPixmap& image)
{
    QSize labelSize = imgSlot->size();
    QSize imageSize = image.size();
    if (imageSize.width() > labelSize.width() || imageSize.height() > labelSize.height()) {
        imageSize.scale(labelSize, Qt::KeepAspectRatio);
        image =image.scaled(imageSize, Qt::KeepAspectRatio);
    }
}

void page1::uploadImg(cv::Mat img, QImage::Format format)
{
    QImage qimage2(img.data, img.cols, img.rows,format);
    QPixmap outputPix = QPixmap::fromImage(qimage2);

    rescaleImg(ui->image2, outputPix);
    ui->image2->setPixmap(outputPix);
}


void page1::on_optionCombo_currentIndexChanged(int index)
{
    if(ui->operationBox->currentIndex()==0){
        if(index ==0){
            ui->stackedWidget->setCurrentIndex(0);
            ui->label1_1->setText("Noise Value");
        }
        else if(index==1){
            ui->stackedWidget->setCurrentIndex(2);
            ui->label3_1->setText("Set Mean");
            ui->label3_2->setText("Set Std");
        }
        else if(index==2){
            ui->stackedWidget->setCurrentIndex(1);
            ui->label2_1->setText("Noise Probability");
        }
    } else if(ui->operationBox->currentIndex()==1){
        ui->stackedWidget->setCurrentIndex(0);
        ui->label1_1->setText("Kernal Size");
        if (index==2)
            ui->stackedWidget->setCurrentIndex(4);
        else if(index==0)
            ui->stackedWidget->setCurrentIndex(5);
    } else if(ui->operationBox->currentIndex()==2){
        if(index==2)
            ui->stackedWidget->setCurrentIndex(3);

        else{
            ui->label1_1->setText("Kernal Size");
            ui->stackedWidget->setCurrentIndex(0);
        }

    }
    updateOutput();
}


void page1::on_intSpinner_valueChanged(int arg1)
{
    updateOutput();
}


void page1::on_doubleSpinBox_valueChanged(double arg1)
{
    updateOutput();
}


void page1::on_intspin3_2_valueChanged(int arg1)
{
    updateOutput();
}


void page1::on_intspin3_1_valueChanged(int arg1)
{
    updateOutput();
}


void page1::on_operationBox_currentIndexChanged(int index)
{
    ui->optionCombo->clear();
    ui->intSpinner->setMinimum(0);
    if(index==0){
        ui->optionCombo->insertItem(0, "Uniform");
        ui->optionCombo->insertItem(1, "Gaussian");
        ui->optionCombo->insertItem(2, "Salt & Pepper");
    }else if(index==1) {
        ui->optionCombo->insertItem(0, "Sobel");
        ui->optionCombo->insertItem(1, "Roberts");
        ui->optionCombo->insertItem(2, "Canny");
        ui->optionCombo->insertItem(3, "Perwitt");
        ui->intSpinner->setMinimum(1);
    }else if(index==2) {
        ui->optionCombo->insertItem(0, "Average");
        ui->optionCombo->insertItem(1, "Median");
        ui->optionCombo->insertItem(2, "Gaussian");
        ui->intSpinner->setMinimum(2);
    }
    updateOutput();
}


void page1::on_intspin5_1_valueChanged(int arg1)
{
     updateOutput();
}


void page1::on_doubleSpinBox_3_valueChanged(double arg1)
{
    updateOutput();
}



void page1::on_intspin5_2_valueChanged(int arg1)
{
    updateOutput();
}


void page1::on_intspin5_3_valueChanged(int arg1)
{
    updateOutput();
}


void page1::on_doubleSpinBox_2_valueChanged(double arg1)
{
    updateOutput();
}


void page1::on_intspin4_1_valueChanged(int arg1)
{
    updateOutput();
}


void page1::on_dirCombo_currentIndexChanged(int index)
{
    updateOutput();
}


void page1::on_intspin6_1_valueChanged(int arg1)
{
     updateOutput();
}

