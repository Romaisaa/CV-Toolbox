#include "filters.h"

filters::filters()
{

}



void filters::add_uniform_noise(cv::Mat &oldImage, cv::Mat &newImage, int noise)
{
    int width = oldImage.cols;
    int height = oldImage.rows;
    int channels = oldImage.channels();

    cv::Mat noiseMat(height, width, oldImage.type());
    cv::randu(noiseMat, cv::Scalar(-noise), cv::Scalar(noise));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < channels; k++)
            {
                int value = oldImage.at<cv::Vec3b>(i, j)[k] + noiseMat.at<cv::Vec3b>(i, j)[k];
                if (value > 255)
                {
                    value = 255;
                }
                else if (value < 0)
                {
                    value = 0;
                }
                newImage.at<cv::Vec3b>(i, j)[k] = value;
            }
        }
    }
}


void filters::add_gaussian_noise(cv::Mat& oldImage, cv::Mat& newImage, double mean, double stddev)
{
    int width = oldImage.cols;
    int height = oldImage.rows;
    int channels = oldImage.channels();

    // Generate a matrix of normally distributed random numbers
    cv::Mat noise(height, width, CV_64FC(channels));
    randn(noise, mean, stddev);

    // Add the noise matrix to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < channels; k++)
            {
                int value = oldImage.at<cv::Vec3b>(i, j)[k] + noise.at<cv::Vec3d>(i, j)[k];
                if (value > 255)
                {
                    value = 255;
                }
                newImage.at<cv::Vec3b>(i, j)[k] = value;
            }
        }
    }
}


void filters::add_salt_and_pepper_noise(cv::Mat& oldImage, cv::Mat& newImage, float noise_prob)
{
    int width = oldImage.cols;
    int height = oldImage.rows;
    int channels = oldImage.channels();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < channels; k++)
            {
                int pixel_val = oldImage.at<cv::Vec3b>(i, j)[k];
                float rand_num = ((float)rand()) / RAND_MAX;
                if (rand_num < noise_prob)
                {
                    // Assign white pixel
                    newImage.at<cv::Vec3b>(i, j)[k] = 255;
                }
                else if (rand_num > (1.0 - noise_prob))
                {
                    // Assign black pixel
                    newImage.at<cv::Vec3b>(i, j)[k] = 0;
                }
                else
                {
                    // No noise added
                    newImage.at<cv::Vec3b>(i, j)[k] = pixel_val;
                }
            }
        }
    }
}


void filters::generateSobelKernels(int size, cv::Mat &sobel_x, cv::Mat &sobel_y)
{
    sobel_x = cv::Mat(size, size, CV_32F);
    sobel_y = cv::Mat(size, size, CV_32F);

    float sobel_x_data[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    float sobel_y_data[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            sobel_x.at<float>(i, j) = sobel_x_data[(i * size + j) % 9];
            sobel_y.at<float>(i, j) = sobel_y_data[(i * size + j) % 9];
        }
    }
}


void filters::generatePrewittKernels(int kernelSize, cv::Mat& kernelX, cv::Mat& kernelY)
{
    kernelX = cv::Mat::zeros(kernelSize, kernelSize, CV_32SC1);
    kernelY = cv::Mat::zeros(kernelSize, kernelSize, CV_32SC1);

    int mid = kernelSize / 2;

    for (int i = 0; i < kernelSize; i++)
    {
        for (int j = 0; j < kernelSize; j++)
        {
            if (i == mid)
            {
                kernelX.at<int>(i, j) = 0;
                kernelY.at<int>(i, j) = 2 * (j - mid);
            }
            else if (j == mid)
            {
                kernelX.at<int>(i, j) = 2 * (i - mid);
                kernelY.at<int>(i, j) = 0;
            }
        }
    }
}


void filters::generateRobertsKernels(int kernelSize, cv::Mat &kernelX, cv::Mat &kernelY)
{
    // Create the kernels
    kernelX = cv::Mat::zeros(kernelSize, kernelSize, CV_32SC1);
    kernelY = cv::Mat::zeros(kernelSize, kernelSize, CV_32SC1);

    // Set the values of the kernels dynamically according to kernel size
    for (int i = 0; i < kernelSize; i++)
    {
        kernelX.at<int>(i, i) = i * 2 + 1 - kernelSize;
        kernelY.at<int>(i, kernelSize - i - 1) = i * 2 + 1 - kernelSize;
    }
}


void filters::sobelEdgeDetector(const cv::Mat &gray_image, cv::Mat &output_image, int kernel_size, SobelType type)
{

    // Generate Sobel kernels
    cv::Mat sobel_x, sobel_y;
    generateSobelKernels(kernel_size, sobel_x, sobel_y);

    // Apply Sobel filter
    int width = gray_image.cols;
    int height = gray_image.rows;
    int center = kernel_size / 2;

    output_image = cv::Mat::zeros(height, width, CV_8UC1);
    cv::Mat output_x = cv::Mat::zeros(height, width, CV_8UC1);
    cv::Mat output_y = cv::Mat::zeros(height, width, CV_8UC1);

    for (int i = center; i < height - center; ++i)
    {
        for (int j = center; j < width - center; ++j)
        {
            float grad_x_sum = 0.0, grad_y_sum = 0.0;
            for (int k = 0; k < kernel_size; ++k)
            {
                for (int l = 0; l < kernel_size; ++l)
                {
                    grad_x_sum += gray_image.at<uchar>(i + k - center, j + l - center) * sobel_x.at<float>(k, l);
                    grad_y_sum += gray_image.at<uchar>(i + k - center, j + l - center) * sobel_y.at<float>(k, l);
                }
            }
            int grad = abs(grad_x_sum) + abs(grad_y_sum);

            output_x.at<uchar>(i, j) = grad_x_sum;
            output_y.at<uchar>(i, j) = grad_y_sum;
            output_image.at<uchar>(i, j) = grad;
        }
    }
    if (type == X)
    {
        output_image = output_x;
    }
    else if (type == Y)
    {
        output_image = output_y;
    }
}


void filters::prewitt_robertEdgeDetectors(const cv::Mat& inputImage, cv::Mat& outputImage, int kernelSize, DetectorType type)
{
    cv::Mat kernelX, kernelY;
    if(type == ROBERT){
        generateRobertsKernels(kernelSize, kernelX, kernelY);
    }else{
        generatePrewittKernels(kernelSize, kernelX, kernelY);
    }

    int width = inputImage.cols;
    int height = inputImage.rows;

    // Initialize output image with zeros
    outputImage = cv::Mat::zeros(height, width, CV_8UC1);

    // Loop over the image pixels
    for (int i = 0; i < height - kernelSize + 1; i++)
    {
        for (int j = 0; j < width - kernelSize + 1; j++)
        {
            // Compute the x and y gradients using the Prewitt kernels
            int gx = 0, gy = 0;
            for (int k = 0; k < kernelSize; k++)
            {
                for (int l = 0; l < kernelSize; l++)
                {
                    gx += inputImage.at<uchar>(i+k, j+l) * kernelX.at<int>(k, l);
                    gy += inputImage.at<uchar>(i+k, j+l) * kernelY.at<int>(k, l);
                }
            }

            // Compute the magnitude of the gradient
            int magnitude = std::sqrt(gx*gx + gy*gy);

            // Set the output pixel to the magnitude of the gradient
            outputImage.at<uchar>(i+kernelSize/2, j+kernelSize/2) = magnitude;
        }
    }
}


void filters::apply_average_filter(cv::Mat &gray_image, cv::Mat &filtered_image, int filter_size)
{
    int height = gray_image.rows;
    int width = gray_image.cols;
    int half_filter_size = filter_size / 2;

    filtered_image = cv::Mat::zeros(height, width, CV_8UC1);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sum = 0;
            for (int k = -half_filter_size; k <= half_filter_size; k++)
            {
                for (int l = -half_filter_size; l <= half_filter_size; l++)
                {
                    int row = i + k;
                    int col = j + l;
                    if (row >= 0 && row < height && col >= 0 && col < width)
                    {
                        sum += gray_image.at<uchar>(row, col);
                    }
                }
            }
            int num_pixels = filter_size * filter_size;
            filtered_image.at<uchar>(i, j) = sum / num_pixels;
        }
    }
}


void filters::apply_median_filter(cv::Mat &gray_image, cv::Mat &filtered_image, int filter_size)
{
    int rows = gray_image.rows;
    int cols = gray_image.cols;
    int half_filter_size = filter_size / 2;

    filtered_image = cv::Mat::zeros(rows, cols, CV_8UC1);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::vector<uchar> values;

            for (int k = -half_filter_size; k <= half_filter_size; k++)
            {
                for (int l = -half_filter_size; l <= half_filter_size; l++)
                {
                    int x = i + k;
                    int y = j + l;

                    if (x >= 0 && x < rows && y >= 0 && y < cols)
                    {
                        values.push_back(gray_image.at<uchar>(x, y));
                    }
                }
            }

            std::sort(values.begin(), values.end());
            filtered_image.at<uchar>(i, j) = values[values.size() / 2];
        }
    }
}


void filters::apply_gaussian_filter(cv::Mat &gray_image, cv::Mat &filtered_image, int filter_size, double sigma)
{
    if(filter_size % 2 == 0){
        return;
    }

    int rows = gray_image.rows;
    int cols = gray_image.cols;
    int half_filter_size = filter_size / 2;


    filtered_image = cv::Mat::zeros(rows, cols, CV_8UC1);

    // Create the filter
    cv::Mat filter = cv::Mat::zeros(filter_size, filter_size, CV_64F);
    double sum = 0.0;

    for (int i = -half_filter_size; i <= half_filter_size; i++)
    {
        for (int j = -half_filter_size; j <= half_filter_size; j++)
        {
            filter.at<double>(i + half_filter_size, j + half_filter_size) = (1 / (2 * M_PI * sigma * sigma)) * exp(-(i * i + j * j) / (2 * sigma * sigma));
            sum += filter.at<double>(i + half_filter_size, j + half_filter_size);
        }
    }

    // Normalize the filter
    for (int i = 0; i < filter_size; i++)
    {
        for (int j = 0; j < filter_size; j++)
        {
            filter.at<double>(i, j) /= sum;
        }
    }

    // Apply the filter
    for (int i = half_filter_size; i < rows - half_filter_size; i++)
    {
        for (int j = half_filter_size; j < cols - half_filter_size; j++)
        {
            double sum = 0.0;

            for (int k = -half_filter_size; k <= half_filter_size; k++)
            {
                for (int l = -half_filter_size; l <= half_filter_size; l++)
                {
                    sum += gray_image.at<uchar>(i + k, j + l) * filter.at<double>(k + half_filter_size, l + half_filter_size);
                }
            }

            filtered_image.at<uchar>(i, j) = sum;
        }
    }
}


cv::Mat filters::applyGaussianFilter(const cv::Mat &inputImage, int kernelSize, double sigma)
{
    cv::Mat kernel = cv::getGaussianKernel(kernelSize, sigma, CV_32F);
    cv::Mat filteredImage;
    filter2D(inputImage, filteredImage, CV_32F, kernel);
    return filteredImage;
}

cv::Mat filters::applySobelFilter(const cv::Mat &inputImage, int kernelSize, bool horizontal)
{
    cv::Mat filteredImage, kernal_x, kernal_y;
    generateSobelKernels(kernelSize, kernal_x, kernal_y);

    if (horizontal)
    {
        filter2D(inputImage, filteredImage, CV_32F, kernal_x);
    }
    else
    {
        filter2D(inputImage, filteredImage, CV_32F, kernal_y);
    }

    return filteredImage;
}

cv::Mat filters::nonMaxSuppression(const cv::Mat &magnitude, const cv::Mat &direction)
{
    cv::Mat suppressedImage(magnitude.rows, magnitude.cols, CV_32F, cv::Scalar(0));
    float angle, a, b, c;

    for (int i = 1; i < magnitude.rows - 1; i++)
    {
        for (int j = 1; j < magnitude.cols - 1; j++)
        {
            angle = direction.at<float>(i, j);

            if ((angle < 22.5 && angle >= 0) || (angle >= 157.5 && angle < 202.5) || (angle >= 337.5 && angle <= 360))
            {
                a = magnitude.at<float>(i, j - 1);
                b = magnitude.at<float>(i, j + 1);
            }
            else if ((angle >= 22.5 && angle < 67.5) || (angle >= 202.5 && angle < 247.5))
            {
                a = magnitude.at<float>(i - 1, j + 1);
                b = magnitude.at<float>(i + 1, j - 1);
            }
            else if ((angle >= 67.5 && angle < 112.5) || (angle >= 247.5 && angle < 292.5))
            {
                a = magnitude.at<float>(i - 1, j);
                b = magnitude.at<float>(i + 1, j);
            }
            else
            {
                a = magnitude.at<float>(i - 1, j - 1);
                b = magnitude.at<float>(i + 1, j + 1);
            }

            c = magnitude.at<float>(i, j);
            if (c > a && c > b)
            {
                suppressedImage.at<float>(i, j) = c;
            }
        }
    }

    return suppressedImage;
}

// Function to apply double thresholding to the input image
void filters::doubleThreshold(cv::Mat &image, double lowThreshold, double highThreshold)
{
    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
        {
            if (image.at<float>(i, j) < lowThreshold)
            {
                image.at<float>(i, j) = 0;
            }
            else if (image.at<float>(i, j) > highThreshold)
            {
                image.at<float>(i, j) = 255;
            }
            else
            {
                image.at<float>(i, j) = 128;
            }
        }
    }
}

void filters::cannyEdgeDetector(const cv::Mat &gray_image, cv::Mat &output_image, int lowThreshold, int highThreshold, int kernel_size, double sigma)
{
\

    // Apply Gaussian filter
    cv::Mat smoothed_image = applyGaussianFilter(gray_image, kernel_size, sigma);

    // // Apply Sobel filter to get gradients in x and y directions
    cv::Mat grad_x = applySobelFilter(smoothed_image, 3, true);
    cv::Mat grad_y = applySobelFilter(smoothed_image, 3, false);

    // Compute magnitude and direction of gradients
    cv::Mat magnitude, direction;
    magnitude = cv::Mat::zeros(grad_x.rows, grad_x.cols, CV_32F);
    direction = cv::Mat::zeros(grad_x.rows, grad_x.cols, CV_32F);

    for (int i = 0; i < grad_x.rows; ++i)
    {
        for (int j = 0; j < grad_x.cols; ++j)
        {
            float gx = grad_x.at<float>(i, j);
            float gy = grad_y.at<float>(i, j);
            magnitude.at<float>(i, j) = sqrt(gx * gx + gy * gy);
            direction.at<float>(i, j) = atan2(gy, gx) * 180 / M_PI;
        }
    }

    // Apply non-maximum suppression
    cv::Mat suppressed_image = nonMaxSuppression(magnitude, direction);

    // Apply double thresholding
    doubleThreshold(suppressed_image, lowThreshold, highThreshold);

    // Apply edge tracking by hysteresis
    output_image = cv::Mat::zeros(suppressed_image.rows, suppressed_image.cols, CV_8UC1);
    for (int i = 0; i < suppressed_image.rows; ++i)
    {
        for (int j = 0; j < suppressed_image.cols; ++j)
        {
            if (suppressed_image.at<float>(i, j) == 255)
            {
                // Start tracking the edge
                output_image.at<uchar>(i, j) = 255;

                // Check neighbors to see if they are also edges
                for (int ii = i - 1; ii <= i + 1; ++ii)
                {
                    for (int jj = j - 1; jj <= j + 1; ++jj)
                    {
                        if (ii >= 0 && ii < suppressed_image.rows && jj >= 0 && jj < suppressed_image.cols)
                        {
                            if (suppressed_image.at<float>(ii, jj) == 128)
                            {
                                // Mark this neighbor as an edge and continue tracking
                                suppressed_image.at<float>(ii, jj) = 255;
                                output_image.at<uchar>(ii, jj) = 255;
                            }
                        }
                    }
                }
            }
        }
    }
}
