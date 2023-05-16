#include "page11.h"
#include "ui_page11.h"
#include "CV/face_recognition.h"

page11::page11(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page11)
{
    ui->setupUi(this);
    ui->train_pca_btn->setEnabled(false);
    ui->predict_btn->setEnabled(false);
    ui->operation_stack->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
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
        if(trained)
            ui->predict_btn->setEnabled(true);

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



void page11::readImages(std::string folderPath, cv::Mat& images, std::vector<std::string>& labels,bool isTest) {
    int row = 0, col = 0;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if  (entry.is_regular_file() && entry.path().extension().string() == ".png") {
            row++;
        }
        if (row == 1) {
            cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_GRAYSCALE);
            img = img.reshape(1, 1);
            col = img.size[1];
        }
    }
    images = cv::Mat(row, col, CV_8U);
    int idx = 0;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if  (entry.is_regular_file() && entry.path().extension().string() == ".png") {
             cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_GRAYSCALE);
            img = img.reshape(1, 1);
            cv::Mat row = images.row(idx);
            img.copyTo(row);
            idx++;
            std::vector<std::string> splitParts;
            std::istringstream iss(entry.path().filename().string());
            std::string part;

            while (std::getline(iss, part, '_')) {
                splitParts.push_back(part);
            }

            if (!splitParts.empty()) {
                std::string firstPart = splitParts[0];
                if(personToPathMapper.count(firstPart)==0){
                        personToPathMapper[firstPart]=entry.path().string();
            }
               labels.push_back(firstPart);
            }
        }
    }
    if(!isTest){
        QStringList comboItems;
        ui->comboBox_2->clear();

        for (auto it = personToPathMapper.begin(); it != personToPathMapper.end(); it++)
        {
            comboItems.push_front(QString::fromStdString(it->first));
        }
        ui->comboBox_2->addItems(comboItems);

    }
    images.convertTo(images, CV_32F);
}


void page11::on_select_folder_btn_clicked()
{

    folderPath = QFileDialog::getExistingDirectory(nullptr, "Select Folder", QDir::homePath());
    if(!folderPath.isEmpty()){
    for (const auto& entry : fs::directory_iterator(folderPath.toStdString()))
        {
            if (entry.is_regular_file() && entry.path().extension().string() == ".json")
            {
               filePath=entry.path().string();
               fileExist=true ;
               break;
            }
        }
    ui->train_pca_btn->setEnabled(true);
    ui->path_label->setText(folderPath);
    }


}


void page11::on_train_pca_btn_clicked()
{

    cv::Mat images;
    std::vector<std::string> labels;
    readImages(folderPath.toStdString(),images,labels,false);
    qDebug()<<"read";
    fr=new face_recognition();
    if(fileExist)
       fr->setModel(filePath,images,labels);
    else{
       fr->train(images,labels);
       fr->saveModel(folderPath.toStdString()+"/model.json");
    }
    trained=true;
   if(!fileName.isEmpty())
        ui->predict_btn->setEnabled(true);
}


void page11::on_predict_btn_clicked()
{
    if(!fileName.isEmpty()){
        cv::Mat img_gray=cv::imread(fileName.toStdString(),cv::IMREAD_GRAYSCALE);
        std::string label=fr->predictPerson(img_gray);
        label=personToPathMapper[label];
        cv::Mat predictedImg = cv::imread(label,cv::IMREAD_GRAYSCALE);
        QImage qimage(predictedImg.data, predictedImg.cols, predictedImg.rows, QImage::Format_Grayscale8);
        QPixmap pred_image  = QPixmap::fromImage(qimage);
        int w = ui->outputImageLabel->width();
        int h = ui->outputImageLabel->height();
        ui->outputImageLabel->setPixmap(pred_image.scaled(w,h,Qt::KeepAspectRatio));
        ui->outputImageLabel->setScaledContents(true);

    }
}


void page11::on_optionsBox_currentIndexChanged(int index)
{
        ui->operation_stack->setCurrentIndex(index);
}


void page11::on_n_comp_valueChanged(int arg1)
{
    fr->setNComponent(arg1);
}


void page11::on_comboBox_currentIndexChanged(int index)
{
    if(index==0 &&trained &&!fileName.isEmpty())
        ui->predict_btn->setEnabled(false);
    else
        ui->predict_btn->setEnabled(true);
    ui->stackedWidget->setCurrentIndex(index);
}


void page11::on_testUploadBtn_clicked()
{
      QString testFolderPath = QFileDialog::getExistingDirectory(nullptr, "Select Folder", QDir::homePath());
      cv::Mat testImages;
      if(!testFolderPath.isEmpty())
      {
          std::vector<std::string> testLabels;
          readImages(testFolderPath.toStdString(), testImages,testLabels ,true);
          fr->testImages(testImages, predictions);
          fr->generateROC(predictions,testLabels,ROC);
          ui->comboBox_2->setEnabled(true);
      }


}


void page11::on_comboBox_2_currentIndexChanged(int index)
{
   //plotter::plotROC(ui->widget,ROC[index]);
// fr->personToLabelmapper
}

