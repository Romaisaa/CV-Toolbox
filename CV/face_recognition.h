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
   private:
    void performPCA(cv::Mat& dataPoints, cv::Mat& eigenvalues, cv::Mat& eigenvectors, cv:: Mat& convertedData,int numComponents);
    void multiplyEigen(cv::Mat& eigenvectors, cv::Mat& image, cv::Mat& result);


};

#endif // FACE_RECOGNITION_H
