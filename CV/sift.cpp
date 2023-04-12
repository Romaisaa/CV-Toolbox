#include "sift.h"



//Convert pixel representation from char to float
// Image Normalization
void Sift:: char_to_float(cv::Mat& Image1, cv::Mat&Image2) {
    Image2 = cv::Mat::zeros(Image1.rows, Image1.cols, CV_32FC1);

    for (int i = 0;i < Image1.rows;i++) {
        for (int j = 0;j < Image1.cols;j++) {
            Image2.at<float>(i, j) = Image1.at<uchar>(i, j) / 255.0;
        }
    }
}

//Subsample an image by a given factor
cv::Mat subsample(cv::Mat& I, float factor) {
    cv::Mat tmp = I, dst = I;

    pyrDown(tmp, dst, cv::Size(tmp.cols / factor, tmp.rows / factor));

    return dst;
}

//Get a gaussian pyramid from an image
vector<cv::Mat> get_pyramid(cv::Mat& I, int levels) {
    cv::Mat tmp = I, dst = I;
    vector<cv::Mat> pyramid;

    int level = 0;
    pyramid.push_back(dst);

    level++;

    while (level < levels) {
        dst = subsample(tmp, 2);
        pyramid.push_back(dst);
        level++;
        tmp = dst;
    }

    return pyramid;
}

//Get the scale space
void Sift:: get_scale_space(cv::Mat& I, float sigma,vector<vector<cv::Mat>>& scale_space,float k) {
    vector<cv::Mat> pyramid = get_pyramid(I, 4);
    for (int i = 0;i < 4;i++) {
        GaussianBlur(pyramid[i], pyramid[i], cv::Size(0, 0), sigma);
        scale_space[i][0] = pyramid[i];

        for (int j = 1;j < 5;j++) {
            cv::Mat aux;
            GaussianBlur(pyramid[i], aux, cv::Size(0, 0), pow(k, j) * sigma);
            scale_space[i][j] = aux;
        }
    }
}

//Get the difference of gaussians
void Sift:: get_DoG(vector<vector<cv::Mat>>& scale_space, vector<vector<cv::Mat>>& DoG) {


    for (int i = 0;i < 4;i++) {
        for (int j = 0;j < 4;j++) {
            DoG[i][j] = abs(scale_space[i][j + 1] - scale_space[i][j]);
        }
    }

}

//Auxiliar function to get the keypoints
bool is_max(cv::Mat& I1, cv::Mat& I2, cv::Mat& I3, int row, int col) {
    for (int i = -1;i <= 1;i++) {
        for (int j = -1;j <= 1;j++) {
            if (i == 0 && j == 0) continue;

            if (I2.at<float>(row, col) < I2.at<float>(row + i, col + j)) return false;
        }
    }

    for (int i = -1;i <= 1;i++) {
        for (int j = -1;j <= 1;j++) {
            if (I2.at<float>(row, col) < I1.at<float>(row + i, col + j) ||
                I2.at<float>(row, col) < I3.at<float>(row + i, col + j)) return false;
        }
    }

    return true;
}

//Auxiliar function to get the keypoints
bool is_min(cv::Mat& I1, cv::Mat& I2, cv::Mat& I3, int row, int col) {
    for (int i = -1;i <= 1;i++) {
        for (int j = -1;j <= 1;j++) {
            if (i == 0 && j == 0) continue;

            if (I2.at<float>(row, col) > I2.at<float>(row + i, col + j)) return false;
        }
    }

    for (int i = -1;i <= 1;i++) {
        for (int j = -1;j <= 1;j++) {
            if (I2.at<float>(row, col) > I1.at<float>(row + i, col + j) ||
                I2.at<float>(row, col) > I3.at<float>(row + i, col + j)) return false;
        }
    }

    return true;
}

//Get the keypoints
void Sift:: get_keypoints(vector<vector<cv::Mat>>& DoG, vector<vector<cv::Mat>>&keypoints) {


    for (int i = 0;i < 4;i++) {
        cv::Mat first = DoG[i][0];
        cv::Mat second = DoG[i][1];
        cv::Mat third = DoG[i][2];
        cv::Mat fourth = DoG[i][3];
        cv::Mat keypoints1 = cv::Mat::zeros(first.rows, first.cols, CV_32FC1);
        cv::Mat keypoints2 = cv::Mat::zeros(first.rows, first.cols, CV_32FC1);

        for (int row = 1; row < second.rows - 1; row++) {
            for (int col = 1; col < second.cols - 1; col++) {
                if (is_max(first, second, third, row, col) || is_min(first, second, third, row, col))
                    keypoints1.at<float>(row, col) = 1;

                if (is_max(second, third, fourth, row, col) || is_min(second, third, fourth, row, col))
                    keypoints2.at<float>(row, col) = 1;
            }
        }

        keypoints[i][0] = keypoints1;
        keypoints[i][1] = keypoints2;
    }

}

//Remove low contrast keypoints
void Sift:: remove_low_contrast(vector<vector<cv::Mat>>& keypoints, vector<vector<cv::Mat>>& DoG, float contrast_threshold) {
    for (int i = 0;i < 4;i++) {

        cv::Mat second = DoG[i][1];
        cv::Mat third = DoG[i][2];

        for (int row = 0; row < second.rows; row++) {

            for (int col = 0; col < second.cols; col++) {

                if (keypoints[i][0].at<float>(row, col) == 1 && second.at<float>(row, col) < contrast_threshold)

                    keypoints[i][0].at<float>(row, col) = 0;

                if (keypoints[i][1].at<float>(row, col) == 1 && third.at<float>(row, col) < contrast_threshold)

                    keypoints[i][1].at<float>(row, col) = 0;
            }
        }
    }
}

//Remove edges from the keypoints
void Sift:: remove_edges(vector<vector<cv::Mat>>& keypoints, vector<vector<cv::Mat>>& DoG, float edge_threshold) {
    for (int i = 0;i < 4;i++) {
        cv::Mat second = DoG[i][1];
        cv::Mat third = DoG[i][2];

        for (int row = 1;row < second.rows - 1; row++) {
            for (int col = 1; col < second.cols - 1; col++) {
                float dxx = second.at<float>(row, col + 1) + second.at<float>(row, col - 1) - 2 * second.at<float>(row, col);
                float dyy = second.at<float>(row + 1, col) + second.at<float>(row - 1, col) - 2 * second.at<float>(row, col);
                float dxy = (second.at<float>(row + 1, col + 1) + second.at<float>(row - 1, col - 1) - second.at<float>(row - 1, col + 1) - second.at<float>(row + 1, col - 1)) / 4;
                float trH = dxx + dyy;
                float detH = dxx * dyy - dxy * dxy;

                if (detH < 0 || ((trH * trH) / detH) >(((edge_threshold + 1) * (edge_threshold + 1)) / edge_threshold)) keypoints[i][0].at<float>(row, col) = 0;

                dxx = third.at<float>(row, col + 1) + third.at<float>(row, col - 1) - 2 * third.at<float>(row, col);
                dyy = third.at<float>(row + 1, col) + third.at<float>(row - 1, col) - 2 * third.at<float>(row, col);
                dxy = (third.at<float>(row + 1, col + 1) + third.at<float>(row - 1, col - 1) - third.at<float>(row - 1, col + 1) - third.at<float>(row + 1, col - 1)) / 4;
                trH = dxx + dyy;
                detH = dxx * dyy - dxy * dxy;

                if (detH < 0 || ((trH * trH) / detH) >(((edge_threshold + 1) * (edge_threshold + 1)) / edge_threshold)) keypoints[i][1].at<float>(row, col) = 0;
            }
        }
    }
}


//Draw keypoints
void Sift:: draw_keypoints(cv::Mat& I, vector<vector<cv::Mat>>& keypoints) {
    for (int k = 0;k < 4;k++) {
        for (int i = 0;i < keypoints[k][0].rows;i++) {
            for (int j = 0;j < keypoints[k][0].cols;j++) {
                if (keypoints[k][0].at<float>(i, j) == 1.0 || keypoints[k][1].at<float>(i, j) == 1.0) {
                    if (k == 0) circle(I, cv::Point(((int)pow(2, k)) * j, ((int)pow(2, k)) * i), 8, cv::Scalar(255, 0, 0));
                    else if (k == 1) circle(I, cv::Point(((int)pow(2, k)) * j, ((int)pow(2, k)) * i), 7, cv::Scalar(0, 255, 0));
                    else if (k == 1) circle(I, cv::Point(((int)pow(2, k)) * j, ((int)pow(2, k)) * i), 6, cv::Scalar(0, 0, 255));
                    else if (k == 2) circle(I, cv::Point(((int)pow(2, k)) * j, ((int)pow(2, k)) * i), 5, cv::Scalar(255, 255, 0));
                    else circle(I, cv::Point(((int)pow(2, k)) * j, ((int)pow(2, k)) * i), 4, cv::Scalar(0, 255, 255));
                }
            }
        }
    }
}




void Sift:: sift_keypoints(cv::Mat& input_img,cv::Mat& keypoints_img,float sigma, float contrast_threshold, float edge_threshold, float k) {
    cv::Mat gray_img_converted;
    vector<vector<cv::Mat>> scale_space(4, vector<cv::Mat>(5));
    vector<vector<cv::Mat>> DoG(4, vector<cv::Mat>(4));
    vector<vector<cv::Mat>> keypoints(4, vector<cv::Mat>(2));


    char_to_float(input_img, gray_img_converted);
    get_scale_space(gray_img_converted, sigma,scale_space,k);
    get_DoG(scale_space,DoG);
    get_keypoints(DoG,keypoints);
    remove_low_contrast(keypoints, DoG,contrast_threshold);
    remove_edges(keypoints, DoG, edge_threshold);
    keypoints_img = input_img.clone();
    draw_keypoints(keypoints_img, keypoints);


}
