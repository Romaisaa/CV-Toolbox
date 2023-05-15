#include "segmenation_page.h"
#include "ui_page10.h"

Page10::Page10(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page10)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->aggProgressBar->setValue(0);
}

Page10::~Page10()
{
    delete ui;
}


void Page10::on_applyBtn_clicked()
{
    cv::Mat output(img.size(), img.type());
    if(ui->type_comboBox->currentIndex()==0)
        Segmentation::kmeansSegmentation(img, output, ui->kValueInput->value());
    else if (ui->type_comboBox->currentIndex()==1)
        //Mean Shift
         Segmentation::meanShiftSegmentation(img,output,ui->meanColorRaduis->value(),ui->meanSpatialRaduis->value(),ui->meanMaxItr->value());
    else if (ui->type_comboBox->currentIndex()==2)
        //Agglomarative
         Segmentation::agglomarativeSegmentation(img,output,ui->aggClusters->value(),ui->aggThreshold->value(),ui->aggProgressBar);
    else if (ui->type_comboBox->currentIndex()==3)
    {
        cv::Mat inputImage = cv::imread(Img.toStdString(), cv::IMREAD_COLOR);
        if(this->scene->seeds.size() > 0){
            Segmentation::regionGrowing(inputImage, output, this->scene->seeds, ui->thersholdValue->value());
        }
    }
    QImage qimage(output.data, output.cols, output.rows, QImage::Format_BGR888);
    QPixmap Output  = QPixmap::fromImage(qimage);
    int w = ui->output_image_label->width();
    int h = ui->output_image_label->height();
    ui->output_image_label->setPixmap(Output.scaled(w,h,Qt::KeepAspectRatio));
    ui->output_image_label->setScaledContents(true);
}

void Page10::on_inputLabel_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!fileName.isEmpty()) {
        // read colored image
        img = cv::imread(fileName.toStdString());
        QImage qimage(img.data, img.cols, img.rows, QImage::Format_BGR888);
        image  = QPixmap::fromImage(qimage);
        int w = ui->inputLabel->width();
        int h = ui->inputLabel->height();
        ui->inputLabel->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));
        ui->inputLabel->setScaledContents(true);
    }
}


void Page10::on_type_comboBox_currentIndexChanged(int index)
{
    if(index == 0 || index == 1 || index == 2){
        ui->stackedWidget_2->setCurrentIndex(0);
    }else{
        ui->stackedWidget_2->setCurrentIndex(1);
    }
    ui->stackedWidget->setCurrentIndex(index);
}


void Page10::on_uploadImage_clicked()
{
    Img= QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    QImage image(Img);
    scene = new Scene(true);
    scene->setRad(5);
    updateImage();
}

void Page10::updateImage(){
    if (!Img.isEmpty()) {
        QImage image(Img);
        scene->addPixmap(QPixmap::fromImage(image));
        ui->graphicsView->setScene(scene);
        ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    }
}

