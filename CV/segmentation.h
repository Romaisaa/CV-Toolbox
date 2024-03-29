#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/hal/hal.hpp>
#include <thread>
#include <mutex>
#include <QDebug>
#include <QProgressBar>

class Segmentation
{
public:
    static void meanShiftSegmentation(const cv::Mat& inputImage, cv::Mat& outputImage, float colorRaduis, float spatialRadius, int maxIterations);
    static void agglomarativeSegmentation(cv::Mat& originalImage, cv::Mat& segmented_img, int num_clusters, float dist_threshold, QProgressBar* progress_bar);
    static void kmeansSegmentation(cv::Mat inputImage, cv::Mat& outputImage, int K);
    static void regionGrowing(cv::Mat inputImage, cv::Mat& outputImage, std::vector<cv::Point> seedPoints, int threshold);

private:
    static float distance(cv::Vec3b a, cv::Vec3b b);
    static cv::Vec3b merge(cv::Vec3b a, cv::Vec3b y);
    static void closetClusters(std::vector<cv::Vec3b> clusters, std::vector<cv::Vec3d>& minDistances, std::mutex& myMutex, int start_i, int end_i);
    static double generateKClusters(cv::InputArray _data, int K, cv::InputOutputArray _bestLabels, cv::TermCriteria criteria, int attempts, cv::OutputArray _centers);
    static void createRegion(cv::Mat input, cv::Mat& output, std::vector<std::vector<cv::Point>> &regions, std::vector<cv::Point> seeds, int threshold);
    static void bgr_to_luv(cv::Mat& img_bgr, cv::Mat& output);
};
#endif // SEGMENTATION_H
