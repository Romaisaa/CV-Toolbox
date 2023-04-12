#ifndef SIFT_H
#define SIFT_H



#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <QProgressBar>
#include <thread>
#include <mutex>
using namespace std;

class Sift
{
public:
    static void sift_keypoints(cv::Mat& input_img,cv::Mat& keypoints_img,float sigma, float contrast_threshold, float edge_threshold, float k);



private:
    static void char_to_float(cv::Mat& Image1, cv::Mat&Image2);
    static cv::Mat subsample(cv::Mat& I, float factor);
    static std::vector<cv::Mat> get_pyramid(cv::Mat& I, int levels);
    static void get_scale_space(cv::Mat& I, float sigma,vector<vector<cv::Mat>>& scale_space,float k);
    static void get_DoG(vector<vector<cv::Mat>>& scale_space, vector<vector<cv::Mat>>& DoG);
    static bool is_max(cv::Mat& I1, cv::Mat& I2, cv::Mat& I3, int row, int col);
    static bool is_min(cv::Mat& I1, cv::Mat& I2, cv::Mat& I3, int row, int col);
    static void get_keypoints(vector<vector<cv::Mat>>& DoG, vector<vector<cv::Mat>>&keypoints);
    static void remove_low_contrast(vector<vector<cv::Mat>>& keypoints, vector<vector<cv::Mat>>& DoG, float contrast_threshold);
    static void remove_edges(vector<vector<cv::Mat>>& keypoints, vector<vector<cv::Mat>>& DoG, float edge_threshold);
    static void draw_keypoints(cv::Mat& I, vector<vector<cv::Mat>>& keypoints);

};



#endif // SIFT_H
