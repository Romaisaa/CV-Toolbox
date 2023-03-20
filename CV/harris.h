#ifndef HARRIS_H
#define HARRIS_H
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

class Harris
{
public:
    static void harrisCornerDetector(const cv::Mat& inputImage, cv::Mat& outputImage, double threshold, int windowSize);
private:
    static void plotCorners(const cv::Mat& input, cv::Mat& output, cv::Mat& harrisResponse, int threshold);
    static void convolution(const cv::Mat& src, cv::Mat& dst, const cv::Mat& kernel);
    static void boxFilter(const cv::Mat& src, cv::Mat& dst, cv::Size ksize);

};

#endif // HOUGH_H
