#include "face_recognition.h"
#include <QDebug>
#include <iostream>


face_recognition::face_recognition()
{

}

void face_recognition::setModel(std::string filePath, cv::Mat images, std::vector<std::string> labels)
{
    cv::FileStorage fs(filePath, cv::FileStorage::READ);
    fs["eigenValues"] >> eigenValues;
    fs["eigenVectors"] >> eigenVectors;
    fs.release();
    this->labels = labels;
    this->images = images;
    fitByNComp();
}

void face_recognition::train(cv::Mat images, std::vector<std::string> labels)
{
    performPCA(images);
    this->labels = labels;
    this->images = images;
    fitByNComp();

}

void face_recognition::setNComponent(int n_component)
{
    this->n_component = n_component;

}
void face_recognition::fitByNComp(){
    cv::Mat tempVectors = eigenVectors.rowRange(0, this->n_component);
    multiplyEigen(tempVectors, this->images, this->transformedImages);
    trainImages(this->transformedImages);
}

cv::Mat face_recognition::transformImage(cv::Mat image){

    cv::Mat tempEigen = eigenVectors.rowRange(0, n_component);
    cv::Mat output;
    multiplyEigen(tempEigen, image, output);
    return output;
}

void face_recognition:: performPCA(cv::Mat& dataPoints)
{
    cv::Mat mean;
    cv::reduce(dataPoints, mean, 0, cv::REDUCE_AVG);
    for (int i = 0; i < dataPoints.rows; i++)
    {
        dataPoints.row(i) = dataPoints.row(i) - mean;
    }
    cv::Mat covariance;
    cv::mulTransposed(dataPoints, covariance, true);
    covariance = covariance / (dataPoints.rows - 1);
    cv::eigen(covariance, this->eigenValues, this->eigenVectors);
}

void face_recognition:: multiplyEigen(cv::Mat& eigenvectors, cv::Mat& images, cv::Mat& result)
{

    result = images * eigenvectors.t();
}


void face_recognition:: saveModel(std::string filePath) {
    cv::FileStorage fs(filePath, cv::FileStorage::WRITE);
    fs << "eigenValues" << eigenValues;
    fs << "eigenVectors" << eigenVectors;
    fs.release();
}

void face_recognition::trainImages(cv::Mat images) {


    cv::Mat y(images.rows, 1, images.type());
    int labeller = 0;
    for (int i = 0; i < labels.size(); i++) {
        if(labelToPersonMapper.count(labeller)==0){
            labelToPersonMapper[labeller] = labels[i];
        }
        if (personToLabelmapper.count(labels[i]) == 0) {
            personToLabelmapper[labels[i]] = labeller;

            labeller++;
        }

        y.at<float>(i) = personToLabelmapper[labels[i]];
    }

    for (int i = 0; i < labeller; i++) {
        cv::Mat temp_y = y.clone();
        for (int j = 0; j < temp_y.rows; j++) {
            if (temp_y.at<float>(j) == i)
                temp_y.at<float>(j) = 1;
            else
                temp_y.at<float>(j) = 0;
        }
        cv::Mat temp_X = images.clone();
        this->models.push_back(LogisticRegression(0.01, 200));
        this->models[i].fit(temp_X, temp_y);

    }

}

void face_recognition::testImages(cv::Mat images, std::vector<cv::Mat>& predictions) {
    cv::Mat testTransformed=transformImage(images);
    for (int i = 0; i < this->models.size(); i++) {
        cv::Mat y_pred = models[i].predict(testTransformed);
        predictions.push_back(y_pred);
    }

}
std::string face_recognition::predictPerson(cv::Mat image){
    std::vector<cv::Mat> predictions;
    cv::Mat reshapedImage = image.reshape(1, 1);
    reshapedImage.convertTo(reshapedImage, CV_32F);
    testImages(reshapedImage,predictions);
    float maxLabel=-1;
    float maxScore=-1;

    for(size_t i=0;i<predictions.size();i++){
        if(predictions[i].at<float>(0)>maxScore){
            maxLabel=i+1;
            maxScore=predictions[i].at<float>(0);
        }
    }
    return labelToPersonMapper[maxLabel];
}

void face_recognition::generateModelConfusion( cv::Mat predictions, std::vector<std::string> truePersons,float threshold, int ModelLabel,float& FPR,float& TPR){
    std::vector<float> trueLabels;
    int TpCounter = 0,TnCounter=0,FnCounter=0,FpCounter=0;
    for (int i = 0; i < truePersons.size(); i++) {
            trueLabels.push_back( personToLabelmapper[truePersons[i]]);
    }
    for(int i=0; i<predictions.rows; i++){
         if(predictions.at<float>(i)>threshold && trueLabels[i]==ModelLabel )
            TpCounter++;

         if(predictions.at<float>(i)<threshold && trueLabels[i]==ModelLabel )
            FnCounter++;

         if(predictions.at<float>(i)>threshold && trueLabels[i]!=ModelLabel )
            FpCounter++;

         if(predictions.at<float>(i)<threshold && trueLabels[i]!=ModelLabel )
            TnCounter++;
    }



    FPR= FpCounter/(TnCounter+FpCounter);
    TPR =TpCounter/(TpCounter+FnCounter);
}

void face_recognition::generateROC(std::vector <cv::Mat> predictions, std::vector<std::string> truePersons, std::vector<std::pair<std::vector<float>,std::vector<float>>>&ROC){
    float FPR,TPR;
    for(int i=0; i<predictions.size(); i++){
        std::vector<float>FPRs,TPRs;
        for(float threshold=0; threshold<=1;threshold+=0.01){
            generateModelConfusion( predictions[i], truePersons, threshold, i,FPR,TPR);
            FPRs.push_back(FPR);
            TPRs.push_back(TPR);

        }

        ROC.push_back({FPRs,TPRs});
    }
}
