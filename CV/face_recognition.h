#ifndef FACE_RECOGNITION_H
#define FACE_RECOGNITION_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/imgproc/types_c.h>
#include "CV/logisticregression.h"


class face_recognition
{
   public:
    face_recognition();
    void setModel(std::string filePath, cv::Mat images, std::vector<std::string> labels);
    void train(cv::Mat images, std::vector<std::string> labels);
    void setNComponent(int n_component);
    void saveModel(std::string filePath);
    void trainImages(cv::Mat images);
    void testImages(cv::Mat images, std::vector<cv::Mat>& predictions);
    std::string predictPerson(cv::Mat image);
    void generateROC(std::vector <cv::Mat> predictions, std::vector<std::string> truePersons, std::vector<std::pair<std::vector<float>,std::vector<float>>>&ROC);
    std::unordered_map<std::string, int> personToLabelmapper;
    std::unordered_map<int,std::string > labelToPersonMapper;



   private:
    void performPCA(cv::Mat& dataPoints);
    void multiplyEigen(cv::Mat& eigenvectors,cv::Mat& images, cv::Mat& result);
    void fitByNComp();
    void generateModelConfusion( cv::Mat predictions, std::vector<std::string> truePersons,float threshold, int ModelLabel,float &FPR,float &TPR);
    cv::Mat transformImage(cv::Mat image);
    int getNearest(cv::Mat images, cv::Mat image);
    cv::Mat eigenValues, eigenVectors, transformedImages, images;
    std::vector<std::string> labels;
    int n_component = 1000;
    std::vector<LogisticRegression> models;






};

#endif // FACE_RECOGNITION_H
