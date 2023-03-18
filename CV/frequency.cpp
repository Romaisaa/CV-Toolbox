#include "frequency.h"

void Frequency::fourierTransform(cv::Mat& img,cv::Mat& cmplx_output) {
    cv::Mat padded;
    int row = cv::getOptimalDFTSize(img.rows);
    int col = cv::getOptimalDFTSize(img.cols);
    copyMakeBorder(img, padded, 0, row - img.rows, 0, col - img.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    cv::Mat planes[] = { cv::Mat_<float>(padded),cv::Mat::zeros(padded.size(),CV_32F) };
    merge(planes, 2, cmplx_output);
    dft(cmplx_output, cmplx_output, cv::DFT_COMPLEX_OUTPUT);
}


void Frequency::frequencyFilter(cv::Mat& dft_filter, int distance,Frequency:: filterType type)
{
    float val = 0.0;
    if (type == Frequency::HIGH) val = 1.0;
    cv::Mat tmp = cv::Mat(dft_filter.rows, dft_filter.cols, CV_32F);
    cv::Point center = cv::Point(dft_filter.rows / 2, dft_filter.cols / 2);
    double radius;
    for (int i = 0; i < dft_filter.rows;++i) {
        for (int j = 0; j < dft_filter.cols;++j) {
            radius = (double)(sqrt(pow((i - center.x), 2.0) + pow((j - center.y), 2.0)));
            if (radius > distance)
                tmp.at<float>(i, j) = val;
            else
                tmp.at<float>(i, j) = 1.0- val;
        }
    }
    cv::Mat finalMask[] = { tmp,tmp };
    merge(finalMask, 2, dft_filter);
}

void Frequency::fftshift(cv::Mat& magI)
{
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;
    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right
    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

}


void Frequency::applyFilter(cv::Mat& input_img, cv::Mat& imgOutput, Frequency::filterType type, int resolution) {
    cv::Mat complexI;
    fourierTransform(input_img, complexI);
    cv::Mat filter = complexI.clone();
    frequencyFilter(filter, resolution, type);
    fftshift(complexI);
    mulSpectrums(complexI, filter, complexI, 0);
    fftshift(complexI);


    cv::Mat planes[2];
    idft(complexI, complexI);
    split(complexI, planes);
    normalize(planes[0], imgOutput, 0, 1, cv::NORM_MINMAX);
    normalizeOutput(imgOutput,imgOutput);

}

void Frequency::hybirdImage(cv::Mat& low_img,cv:: Mat& high_img, cv::Mat& output, int low_res, int high_res) {
    cv::Size newSize(640, 480); // desired size
    cv::resize(low_img, low_img, newSize);
    cv::resize(high_img, high_img, newSize);
    applyFilter(low_img, low_img, Frequency::LOW, low_res);
    applyFilter(high_img, high_img, Frequency::HIGH, high_res);
    output = cv::Mat::zeros(newSize, low_img.type());
    add(low_img, high_img, output);
    normalizeOutput(output,output);


}

void  Frequency::hybirdImage(cv::Mat& low_img_filtered,cv:: Mat& high_img_filterd, cv::Mat& output){
    cv::Size newSize(640, 480);
    output = cv::Mat::zeros(newSize, low_img_filtered.type());
    add(low_img_filtered, high_img_filterd, output);
    output.convertTo(output, -1, 1, -100);
}

void Frequency::normalizeOutput(cv::Mat&input,cv::Mat& output){
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    minMaxLoc(input, &minVal, &maxVal, &minLoc, &maxLoc);
    double scale = 255 / (maxVal - minVal);
    output = (output - minVal) * scale;
    input.convertTo(output, CV_8U);
}
