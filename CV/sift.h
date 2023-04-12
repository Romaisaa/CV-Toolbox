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
using namespace cv;

class Sift
{
public:
    static void sift_keypoints(cv::Mat& input_img,cv::Mat& keypoints_img,float sigma, float contrast_threshold, float edge_threshold, float k);
    static void SIFT_descriptors(vector<vector<float>>&descriptors, vector<vector<cv::Mat>> scale_space, vector<vector<cv::Mat>>& keypoints,float magnitude_threshold,float sigma,float k);



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
    static void get_luminosity_invariance(vector<vector<float>>& descriptors, float magnitude_threshold);
    static void reduce_large_magnitudes(vector<float>& descriptor,float magnitude_threshold);
    static void normalize_vector(vector<float>& descriptor);

    static void get_descriptors(vector<vector<cv::Mat>>& orientations, vector<vector<cv::Mat>>& magnitudes, vector<vector<vector<vector<vector<float>>>>>& keypoints_orientations, vector<vector<float>>& descriptors) ;
    static int get_pos_histogram2(float ang);
    static pair<int, int> get_initial_pos(int i, int j);
    static cv::Mat renew_orientations(Mat& I, int row_begin, int row_end, int col_begin, int col_end, float keypoint_orientation);
    static cv::Mat get_gaussian_circle(Mat& I, int row_begin, int row_end, int col_begin, int col_end, float var);
    static float get_gaussian_weight(int row, int col, int row_begin, int row_end, int col_begin, int col_end, float var);
    static void get_keypoints_orientations(vector<vector<Mat>>& magnitudes, vector<vector<Mat>>& orientations, vector<vector<Mat>>& keypoints, vector<vector<vector<vector<vector<float>>>>> &final_keypoints,float sigma, float k) ;
    static int get_pos_histogram1(float ang);
    static int get_kernel_size(float var);
    static void get_orientations(vector<vector<Mat>>& scale_space, vector<vector<Mat>> &orientations);
    static void get_magnitudes(vector<vector<Mat>>& scale_space, vector<vector<Mat>>& magnitudes,float sigma,float k);
    static float get_angle(float x, float y);


};



#endif // SIFT_H
