#include "sift.h"

const float PI = acos(-1.0);

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
cv::Mat Sift:: subsample(cv::Mat& I, float factor) {
    cv::Mat tmp = I, dst = I;

    pyrDown(tmp, dst, cv::Size(tmp.cols / factor, tmp.rows / factor));

    return dst;
}

//Get a gaussian pyramid from an image
vector<cv::Mat>Sift:: get_pyramid(cv::Mat& I, int levels) {
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
    vector<cv::Mat> pyramid = Sift::get_pyramid(I, 4);
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
bool Sift::is_max(cv::Mat& I1, cv::Mat& I2, cv::Mat& I3, int row, int col) {
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
bool Sift:: is_min(cv::Mat& I1, cv::Mat& I2, cv::Mat& I3, int row, int col) {
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




void Sift:: sift_keypoints(Mat& input_img,Mat& keypoints_img, vector<vector<Mat>>&keypoints, vector<vector<Mat>>&scale_space,float sigma, float contrast_threshold, float edge_threshold, float k) {
    //Handleing / make sure that input is gray scaled image nd convert to mat<float>
    Mat gray_img_converted;
    char_to_float(input_img, gray_img_converted);
    scale_space= vector<vector<Mat>>(4, vector<Mat>(5));
    get_scale_space(gray_img_converted, sigma,scale_space,k);
    vector<vector<Mat>> DoG(4, vector<Mat>(4));
    get_DoG(scale_space,DoG);
    keypoints= vector<vector<Mat>>(4, vector<Mat>(2));
    get_keypoints(DoG,keypoints);
    remove_low_contrast(keypoints, DoG,contrast_threshold);
    remove_edges(keypoints, DoG, edge_threshold);
    keypoints_img = input_img.clone();
    draw_keypoints(keypoints_img, keypoints);


}



float Sift::get_angle(float x, float y) {
    float ang = atan2(y, x);

    if (ang < 0) ang += 2 * PI;

    return (ang * 180.0) / PI;
}

//Get magnitudes of gradients in the scale space
void Sift:: get_magnitudes(vector<vector<Mat>>& scale_space, vector<vector<Mat>>& magnitudes,float sigma,float k) {
    magnitudes = vector<vector<Mat>>(4, vector<Mat>(5));
    for (int i = 0;i < 4;i++) {
        for (int j = 0;j < 5;j++) {
            Mat L = scale_space[i][j];
            Mat mag = Mat::zeros(L.rows, L.cols, CV_32FC1);

            for (int row = 1;row < L.rows - 1;row++) {
                for (int col = 1;col < L.cols - 1;col++) {
                    float dx = L.at<float>(row, col + 1) - L.at<float>(row, col - 1);
                    float dy = L.at<float>(row + 1, col) - L.at<float>(row - 1, col);

                    mag.at<float>(row, col) = sqrt(dx * dx + dy * dy);
                }
            }

            GaussianBlur(mag, mag, Size(0, 0), 1.5 * sigma * pow(k, j));
            magnitudes[i][j] = mag;
        }
    }


}



//Get orientations of gradients in the scale space
void Sift:: get_orientations(vector<vector<Mat>>& scale_space, vector<vector<Mat>> &orientations) {
     orientations= vector<vector<Mat>>(4, vector<Mat>(5));

    for (int i = 0;i < 4;i++) {
        for (int j = 0;j < 5;j++) {
            Mat L = scale_space[i][j];
            Mat orient = Mat::zeros(L.rows, L.cols, CV_32FC1);

            for (int row = 1;row < L.rows - 1;row++) {
                for (int col = 1;col < L.cols - 1;col++) {
                    float dx = L.at<float>(row, col + 1) - L.at<float>(row, col - 1);
                    float dy = L.at<float>(row + 1, col) - L.at<float>(row - 1, col);

                    orient.at<float>(row, col) = get_angle(dx, dy);
                }
            }

            orientations[i][j] = orient;
        }
    }
}

//Get kernel size for the keypoints orientations
int Sift:: get_kernel_size(float var) {
    int size = 39;

    for (int i = 1;i < 20;i++) {
        if (exp(-((float)(i * i)) / (2.0 * var * var)) < 0.001) {
            size = 2 * i - 1;
            break;
        }
    }

    return size;
}

//Get the bin in the histogram
int Sift:: get_pos_histogram1(float ang) {
    for (int i = 0;i < 36;i++) {
        if (ang >= 10.0 * i && ang < 10.0 * (i + 1)) return i;
    }
}

//Get keypoints orientations
void Sift::get_keypoints_orientations(vector<vector<Mat>>& magnitudes, vector<vector<Mat>>& orientations, vector<vector<Mat>>& keypoints, vector<vector<vector<vector<vector<float>>>>> &final_keypoints,float sigma, float k) {
    final_keypoints= vector<vector<vector<vector<vector<float>>>>>(4, vector<vector<vector<vector<float>>>>(5));
    for (int i = 0;i < 4;i++) {
        Mat keypoints1 = keypoints[i][0];
        Mat keypoints2 = keypoints[i][1];

        for (int j = 0;j < 5;j++) {
            int kernel_size = get_kernel_size(sigma * pow(k, j));
            Mat mag = magnitudes[i][j];
            Mat orient = orientations[i][j];
            vector<vector<vector<float>>> keys_mat(mag.rows, vector<vector<float>>(mag.cols));

            for (int row = 0;row < mag.rows - kernel_size;row++) {
                for (int col = 0;col < mag.cols - kernel_size;col++) {
                    if (keypoints1.at<float>(row + (kernel_size / 2), col + (kernel_size / 2)) == 1 || keypoints2.at<float>(row + (kernel_size / 2), col + (kernel_size / 2)) == 1) {
                        vector<float> keys;
                        Mat window_image_mag(mag, Rect(col, row, kernel_size, kernel_size));
                        Mat window_image_orient(orient, Rect(col, row, kernel_size, kernel_size));
                        vector<float> histogram(36);

                        for (int k = 0;k < window_image_mag.rows;k++) {
                            for (int m = 0;m < window_image_mag.cols;m++) {
                                int pos_hist = get_pos_histogram1(window_image_orient.at<float>(k, m));
                                histogram[pos_hist] += window_image_mag.at<float>(k, m);
                            }
                        }

                        float peak = -1.0;

                        for (int k = 0;k < 36;k++) {
                            if (histogram[k] > peak) peak = histogram[k];
                        }

                        for (int k = 0;k < 36;k++) {
                            if (histogram[k] > 0.8 * peak) {
                                keys_mat[row][col].push_back(10.0 * k + 5.0);
                            }
                        }
                    }
                }
            }

            final_keypoints[i][j] = keys_mat;
        }
    }

}

//Get gaussian weights from a gaussian distribution
float Sift::get_gaussian_weight(int row, int col, int row_begin, int row_end, int col_begin, int col_end, float var) {
    float row_med = (1.0 * (row_begin + row_end)) / 2;
    float col_med = (1.0 * (col_begin + col_end)) / 2;
    float row_trans = 1.0 * row - row_med;
    float col_trans = 1.0 * col - col_med;

    return (exp(-(row_trans * row_trans + col_trans * col_trans) / (2.0 * var * var))) / (2.0 * PI * var * var);
}

//Pass a circular gaussian filter through an image
Mat Sift:: get_gaussian_circle(Mat& I, int row_begin, int row_end, int col_begin, int col_end, float var) {
    Mat gaussian_circle = Mat::zeros(row_end - row_begin + 1, col_end - col_begin + 1, CV_32FC1);

    for (int i = 0;i < gaussian_circle.rows;i++) {
        for (int j = 0;j < gaussian_circle.cols;j++) {
            float weight = get_gaussian_weight(row_begin + i, col_begin + j, row_begin, row_end, col_begin, col_end, var);
            gaussian_circle.at<float>(i, j) = weight * I.at<float>(row_begin + i, col_begin + j);
        }
    }

    return gaussian_circle;
}

//Get the orientations of all the neighborhood with respect to the keypoint orientation
Mat Sift:: renew_orientations(Mat& I, int row_begin, int row_end, int col_begin, int col_end, float keypoint_orientation) {
    Mat orientations = Mat::zeros(row_end - row_begin + 1, col_end - col_begin + 1, CV_32FC1);

    for (int i = 0;i < orientations.rows;i++) {
        for (int j = 0;j < orientations.cols;j++) {
            orientations.at<float>(i, j) = I.at<float>(row_begin + i, col_begin + j) - keypoint_orientation;
            if (orientations.at<float>(i, j) < 0) orientations.at<float>(i, j) += 360.0;
        }
    }

    return orientations;
}

//Auxiliar function to run through the neighborhood of a keypoint
pair<int, int> Sift::get_initial_pos(int i, int j) {
    int row, col;

    if (i == 0 || i == 1) row = 4 * i;
    else row = 4 * i + 1;

    if (j == 0 || j == 1) col = 4 * j;
    else col = 4 * j + 1;

    return { row, col };
}

//Get bin in the last histogram
int Sift  ::get_pos_histogram2(float ang) {
    for (int i = 0;i < 8;i++) {
        if (ang >= 45 * i && ang < 45 * (i + 1)) return i;
    }
}

//Get descriptors
void Sift::get_descriptors(vector<vector<Mat>>& orientations, vector<vector<Mat>>& magnitudes, vector<vector<vector<vector<vector<float>>>>>& keypoints_orientations, vector<vector<float>>& descriptors) {
    for (int i = 0;i < 4;i++) {
        for (int j = 0;j < 5;j++) {
            for (int row = 0;row < magnitudes[i][j].rows - 17;row++) {
                for (int col = 0;col < magnitudes[i][j].cols - 17;col++) {
                    if (keypoints_orientations[i][j][row + 8][col + 8].size() > 0) {
                        for (int k = 0;k < (int)keypoints_orientations[i][j][row + 8][col + 8].size();k++) {
                            vector<vector<float>> histograms;
                            Mat gaussian_circle = get_gaussian_circle(magnitudes[i][j], row, row + 16, col, col + 16, 8.5);
                            Mat orient = renew_orientations(orientations[i][j], row, row + 16, col, col + 16, keypoints_orientations[i][j][row + 8][col + 8][k]);

                            for (int k = 0;k < 4;k++) {
                                for (int m = 0;m < 4;m++) {
                                    vector<float> histogram(8);
                                    pair<int, int> initial_pos = get_initial_pos(k, m);
                                    int row_begin = initial_pos.first;
                                    int col_begin = initial_pos.second;

                                    for (int row_inside = row_begin; row_inside < row_begin + 4; row_inside++) {
                                        for (int col_inside = col_begin; col_inside < col_begin + 4; col_inside++) {
                                            int pos_hist = get_pos_histogram2(orient.at<float>(row_inside, col_inside));
                                            histogram[pos_hist] += gaussian_circle.at<float>(row_inside, col_inside);
                                        }
                                    }

                                    histograms.push_back(histogram);
                                }
                            }

                            vector<float> descriptor;

                            for (int k = 0;k < (int)histograms.size();k++) {
                                for (int m = 0;m < (int)histograms[k].size();m++) {
                                    descriptor.push_back(histograms[k][m]);
                                }
                            }

                            descriptors.push_back(descriptor);
                        }
                    }
                }
            }
        }
    }
}

//Normalize descriptors
void Sift::normalize_vector(vector<float>& descriptor) {
    float norm = 0.0;

    for (int i = 0;i < (int)descriptor.size();i++) norm += descriptor[i] * descriptor[i];

    norm = sqrt(norm);

    for (int i = 0;i < (int)descriptor.size();i++) descriptor[i] /= norm;
}

//Filter large magnitudes in the descriptor
void Sift::reduce_large_magnitudes(vector<float>& descriptor,float magnitude_threshold){
    for (int i = 0;i < (int)descriptor.size();i++) descriptor[i] = min(descriptor[i], magnitude_threshold);
}

//Get luminosity invariance in the descriptor
void Sift::get_luminosity_invariance(vector<vector<float>>& descriptors, float magnitude_threshold) {
    for (int i = 0;i < (int)descriptors.size();i++) {
        normalize_vector(descriptors[i]);
        reduce_large_magnitudes(descriptors[i], magnitude_threshold);
        normalize_vector(descriptors[i]);
    }
}

//Wrapper for all the functions previous implemented
void Sift::SIFT_descriptors(vector<vector<float>>&descriptors, vector<vector<Mat>> scale_space, vector<vector<Mat>>& keypoints,float magnitude_threshold,float sigma,float k) {

    vector<vector<Mat>> magnitudes;
    vector<vector<Mat>> orientations;
    vector<vector<vector<vector<vector<float>>>>> keypoints_orientations;
    get_magnitudes(scale_space, magnitudes,sigma,k);
    get_orientations(scale_space, orientations);
    get_keypoints_orientations(magnitudes, orientations, keypoints, keypoints_orientations,sigma,k);
    get_descriptors(orientations, magnitudes, keypoints_orientations, descriptors);
    get_luminosity_invariance(descriptors,magnitude_threshold);

}

vector<KeyPoint> Sift::extract_keypoints(const vector<vector<Mat>>& descriptors) {
    vector<KeyPoint> keypoints;

    // iterate over each element in the vector of vectors
    for (const auto& descriptor : descriptors) {
        // iterate over each Mat in the current vector
        for (const auto& mat : descriptor) {

            // convert the Mat to a vector of keypoints
            vector<KeyPoint> kps;
            Ptr<Feature2D> detector = ORB::create();

            detector->detect(mat, kps);

            // append the keypoints to the output vector
            keypoints.insert(keypoints.end(), kps.begin(), kps.end());
        }
    }

    return keypoints;
}
