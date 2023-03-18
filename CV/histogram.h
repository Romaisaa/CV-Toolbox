#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QVector>

class Histogram
{
public:

    static void getHist(cv::Mat& img, cv::Mat &pdf, cv::Mat &cdf);
    static void histEqualizer(cv::Mat &img, cv::Mat &output);
    static void normalizeImg(cv::Mat &img,cv::Mat& output, int newMin, int newMax);
    static void getMinMax(cv::Mat img, int *min, int *max);
    static void BGR2Gray(cv::Mat &InputImage, cv::Mat &grayImage);
    static void getChannels(cv::Mat &img, cv::Mat &blue, cv::Mat &breen, cv::Mat &red);
    static void globalThreshold(cv::Mat &img, cv::Mat &output);
    static void localThreshold(cv::Mat &img, cv::Mat &output, int pieceSize);
};

#endif // HISTOGRAM_H
