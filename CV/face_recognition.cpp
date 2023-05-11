#include "face_recognition.h"

void face_recognition:: performPCA(cv::Mat& dataPoints, cv::Mat& eigenvalues, cv::Mat& eigenvectors, cv:: Mat& convertedData,int numComponents)
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
    cv::eigen(covariance, eigenvalues, eigenvectors);
    eigenvalues=eigenvalues.rowRange(0, numComponents);
    eigenvectors = eigenvectors.rowRange(0, numComponents);
   convertedData = dataPoints * eigenvectors.t();
}

void face_recognition:: multiplyEigen(cv::Mat& eigenvectors, cv::Mat& image, cv::Mat& result)
{
    cv::Mat reshapedImage = image.reshape(1, 1);
    cv::Mat floatImage;
    reshapedImage.convertTo(floatImage, CV_32F);
    result = floatImage * eigenvectors.t();
}
