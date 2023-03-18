#ifndef FILTERS_H
#define FILTERS_H

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

class filters
{
public:
    filters();
    enum SobelType
    {
        X,
        Y,
        COMBINED
    };

    enum DetectorType {
        ROBERT,
        PERWITT
    };

    // Noise Methods
    static void add_uniform_noise(cv::Mat& oldImage, cv::Mat& newImage, int noise);
    static void add_gaussian_noise(cv::Mat& oldImage, cv::Mat& newImage, double mean, double stddev);
    static void add_salt_and_pepper_noise(cv::Mat& oldImage, cv::Mat& newImage, float noise_prob);

    // Edge Detectors
    static void sobelEdgeDetector(const cv::Mat &input_image, cv::Mat &output_image, int kernel_size, SobelType type);
    static void prewitt_robertEdgeDetectors(const cv::Mat& inputImage, cv::Mat& outputImage, int kernelSize, DetectorType type);
    static void cannyEdgeDetector(const cv::Mat &input_image, cv::Mat &output_image, int lowThreshold, int highThreshold, int kernel_size, double sigma);

    // Image Filter
    static void apply_average_filter(cv::Mat &image, cv::Mat &filtered_image, int filter_size);
    static void apply_median_filter(cv::Mat &image, cv::Mat &filtered_image, int filter_size);
    static void apply_gaussian_filter(cv::Mat &image, cv::Mat &filtered_image, int filter_size, double sigma);

private:
    // Kernals for edge detectors
    static void generateSobelKernels(int size, cv::Mat &sobel_x, cv::Mat &sobel_y);
    static void generatePrewittKernels(int kernelSize, cv::Mat& kernelX, cv::Mat& kernelY);
    static void generateRobertsKernels(int kernelSize, cv::Mat& kernelX, cv::Mat& kernelY);
    static void doubleThreshold(cv::Mat &image, double lowThreshold, double highThreshold);
    static cv::Mat nonMaxSuppression(const cv::Mat &magnitude, const cv::Mat &direction);
    static cv::Mat applyGaussianFilter(const cv::Mat &inputImage, int kernelSize, double sigma);
    static cv::Mat applySobelFilter(const cv::Mat &inputImage, int kernelSize, bool horizontal);

};

#endif // FILTERS_H


