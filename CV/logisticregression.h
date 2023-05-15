#ifndef LOGISTICREGRESSION_H
#define LOGISTICREGRESSION_H


#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
class LogisticRegression
{
public:
    LogisticRegression(double lr, int epochs);

    void fit(cv::Mat X, cv::Mat y);

    cv::Mat predict(cv::Mat X);



private:
    int epochs;
    double lr, b;
    cv::Mat X, y,W, means, std;
    std::vector<double> losses;

    cv::Mat sigmoid(cv::Mat z);

    double loss(cv::Mat y_hat);

    void gradientDescent(cv::Mat y_hat, cv::Mat& dW, double& db);

    void Create_normalize();

    void apply_normalize(cv::Mat X, cv::Mat& output);
};
#endif // LOGISTICREGRESSION_H
