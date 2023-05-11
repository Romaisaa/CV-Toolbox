#ifndef FACE_RECOGNITION_H
#define FACE_RECOGNITION_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/imgproc/types_c.h>


class face_recognition
{
public:
    face_recognition();
    void setModel(std::string filePath, cv::Mat images, std::vector<std::string> labels);
    void train(cv::Mat images, std::vector<std::string> labels);
    void setNComponent(int n_component);
    std::string getPerson(cv::Mat image);

   private:
    void performPCA(cv::Mat& dataPoints);
    void multiplyEigen(cv::Mat& eigenvectors,cv::Mat& images, cv::Mat& result);
    int getNearest(cv::Mat images, cv::Mat image);
    cv::Mat eigenValues, eigenVectors, transformedImages, images;
    std::vector<std::string> labels;
    int n_component = 100;



};

#endif // FACE_RECOGNITION_H
