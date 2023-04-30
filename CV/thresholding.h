#ifndef THRESHOLDING_H
#define THRESHOLDING_H
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

class Thresholding
{
public:
    Thresholding();

    static void normalizeImg(cv::Mat& img, int newMin, int newMax);

    static void getHist(cv::Mat& img, cv::Mat &pdf, cv::Mat &cdf);

    static void histEqualizer(cv::Mat & img, cv::Mat& output);

    static void otsuThresholding(cv::Mat& img, cv::Mat& output);

    static void multiSpectralThreshold(cv::Mat& img, cv::Mat& output);

    static void optimalThresholding(cv::Mat& img, cv::Mat& output);


    static void otsuBackTrack(std::vector<int> threshold, int index, int maxNum, std::vector<std::vector<int>>& output);

    static void localThreshold(cv::Mat& img, cv::Mat& output, int pieceSize);

    static void multiLevelOtsu(cv::Mat img, cv::Mat& output, int M);

private:

    static void getMinMax(cv::Mat img, int* min, int* max);

    static void get_histogram(cv::Mat origin, std::vector<int>& histogram);

    static void normalize_histogram(std::vector<int> histogram, std::vector<double>& norm_hist, int M, int L);

    static void find_valleys(std::vector<double> norm_hist, std::vector<int>& valleys);

    static void threshold_valley_regions(std::vector<int> histogram, std::vector<int>& thresholds, std::vector<int> valleys, int N);

    static int get_thresholds(std::vector<int> histogram);
};

#endif // THRESHOLDING_H
