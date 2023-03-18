#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
class Frequency
{
public:
    enum filterType {LOW,HIGH};
    static void fourierTransform(cv::Mat& img,cv::Mat& cmplx_output);
    static void frequencyFilter(cv::Mat& dft_filter, int distance, filterType type);
    static void fftshift(cv::Mat& magI);
    static void applyFilter(cv::Mat& input_img, cv::Mat& imgOutput, filterType type, int resolution);
    static void hybirdImage(cv::Mat& low_img,cv:: Mat& high_img, cv::Mat& output, int low_res, int high_res);
    static void hybirdImage(cv::Mat& low_img_filtered,cv:: Mat& high_img_filterd, cv::Mat& output);
    static void normalizeOutput(cv::Mat&input,cv::Mat& output);

};

#endif // FREQUENCY_H
