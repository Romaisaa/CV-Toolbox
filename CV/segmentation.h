#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

class Segmentation
{
public:
    static void meanShiftSegmentation(const cv::Mat& inputImage, cv::Mat& outputImage, float colorRaduis, float spatialRadius, int maxIterations);


};
#endif // SEGMENTATION_H
