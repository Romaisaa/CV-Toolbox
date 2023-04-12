#ifndef MATCHING_H
#define MATCHING_H

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

class matching
{
public:
    static void SSD(cv::Mat img, cv::Mat templ, cv::Rect& roi);
    static void crossCorrelation(cv::Mat img, cv::Mat templ, cv::Rect& roi);
    static void matchDes(cv::Mat des1, cv::Mat des2, std::vector < cv::Vec3d>& matches);
};

#endif // MATCHING_H
