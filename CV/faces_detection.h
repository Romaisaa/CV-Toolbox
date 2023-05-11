#ifndef FACES_DETECTION_H
#define FACES_DETECTION_H

#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

class faces_detection
{
public:
    faces_detection();
    static void cropImage(cv::Mat inputImage, cv::Mat& outputImage, cv::Rect faceRect);
    static void detect_faces(cv::Mat& inputImage, cv::Mat& outputImage, bool isCropped);
};

#endif // FACES_DETECTION_H
