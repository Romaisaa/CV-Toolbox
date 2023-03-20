#include "harris.h"

void Harris:: convolution(const cv::Mat& src, cv::Mat& dst, const cv::Mat& kernel)
{
    int ksizeX = kernel.cols;
    int ksizeY = kernel.rows;
    int radiusX = ksizeX / 2;
    int radiusY = ksizeY / 2;

    dst.create(src.size(), src.type());

    for (int i = radiusY; i < src.rows - radiusY; i++)
    {
        for (int j = radiusX; j < src.cols - radiusX; j++)
        {
            float sum = 0;

            for (int k = -radiusY; k <= radiusY; k++)
            {
                for (int l = -radiusX; l <= radiusX; l++)
                {
                    float kernelValue = kernel.at<float>(k + radiusY, l + radiusX);
                    float pixelValue = src.at<float>(i + k, j + l);
                    sum += kernelValue * pixelValue;
                }
            }

            dst.at<float>(i, j) = sum;
        }
    }
}
void Harris:: boxFilter(const cv::Mat& src, cv::Mat& dst, cv::Size ksize)
{
    cv::Mat kernel = cv::Mat::ones(ksize, CV_32F);

    // Normalize the kernel so that the sum of its elements is 1
    kernel /= (float)(ksize.width * ksize.height);

    // Apply the kernel to the source image using a convolution
    convolution(src, dst, kernel);

}
void Harris:: plotCorners(const cv::Mat& input, cv::Mat& output, cv::Mat& harrisResponse, int threshold)
{
    output = input.clone();
    cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);

    for (int i = 0; i < output.rows; i++)
    {
        for (int j = 0; j < output.cols; j++)
        {
            if (harrisResponse.at<float>(i, j) > threshold)
            {
                circle(output, cv::Point(j, i), 1, cv::Scalar(255, 0, 0), 1);
            }
        }
    }
}
void Harris:: harrisCornerDetector(const cv::Mat& inputImage, cv::Mat& outputImage, double threshold, int windowSize)
{
    // Compute the x and y gradients of the image
    cv::Mat gradientX, gradientY;
    cv::Sobel(inputImage, gradientX, CV_32F, 1, 0, windowSize);
    cv::Sobel(inputImage, gradientY, CV_32F, 0, 1, windowSize);

    // Compute the elements of the Harris matrix
    cv::Mat gradientXX = gradientX.mul(gradientX);
    cv::Mat gradientYY = gradientY.mul(gradientY);
    cv::Mat gradientXY = gradientX.mul(gradientY);

    // Compute the sum of the elements in the Harris cv::Matrix
    cv::Mat sumXX, sumYY, sumXY;
    boxFilter(gradientXX, sumXX, cv::Size(windowSize, windowSize));
    boxFilter(gradientYY, sumYY, cv::Size(windowSize, windowSize));
    boxFilter(gradientXY, sumXY, cv::Size(windowSize, windowSize));

    // Compute the determinant and trace of the Harris matrix
    cv::Mat determinant = sumXX.mul(sumYY) - sumXY.mul(sumXY);
    cv::Mat trace = sumXX + sumYY;

    // Compute the Harris response for each pixel
    cv::Mat harrisResponse;
    divide(determinant, trace, harrisResponse);

    // Threshold the Harris response and mark the corners on the output image
    plotCorners(inputImage, outputImage, harrisResponse, threshold);

}
