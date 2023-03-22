#ifndef HOUGH_H
#define HOUGH_H
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <QProgressBar>
#include <thread>
#include <mutex>

class Hough
{
public:
    static void HoughLine(cv::Mat edges, std::vector<cv::Vec2d>& lines, int threshold, double angleRes);
    static void drawLines(cv::Mat& img, std::vector<cv::Vec2d> lines, cv::Scalar color);
    static void HoughCircle(cv::Mat edges, std::vector<cv::Vec3d>& circles,int threshold, int minRadius, int maxRadius, int radiusStep,QProgressBar* bar);
    static void drawCircles(cv::Mat& img, std::vector<cv::Vec3d> circles, cv::Scalar color);
    static void HoughEllipse(cv::Mat img, std::vector<cv::Vec6d>& ellipses, int threshold, int minRadius, int maxRadius, QProgressBar* bar);
    static void drawEllipse(cv::Mat& img, std::vector<cv::Vec6d> ellipses, cv::Scalar color);

private:
    static void oneThreadHough(std::vector<cv::Vec2d> nonZeroImg, std::vector<cv::Vec6d>& ellipses, std::mutex& myMutex, int threshold, int minRadius, int maxRadius, int start, int end);
};

#endif // HOUGH_H
