#include "histogram.h"


void Histogram::getHist(cv::Mat &img, cv::Mat &pdf, cv::Mat &cdf)
{
    for(int row =0; row< img.rows; row++)
         for (int col = 0; col < img.cols; col++) {
            uchar intensity = img.at<uchar>(row, col);
            pdf.at<float>(intensity)= pdf.at<float>(intensity)+1;
        }

    cdf.at<float>(0) = pdf.at<float>(0);

    for (int i = 1; i < cdf.rows; i++) {

            cdf.at<float>(i) = cdf.at<float>(i-1) + pdf.at<float>(i);
        }
}

void Histogram::histEqualizer(cv::Mat& img, cv::Mat& output)
{
    cv::Mat pdf = cv::Mat::zeros(256, 1, CV_32F);
    cv::Mat cdf = cv::Mat::zeros(256, 1, CV_32F);
    output = cv::Mat(img.size(), img.type());
    Histogram::getHist(img, pdf, cdf);
    cdf = cdf / cdf.at<float>(255);
    cdf = cdf * 255;
    cv::Mat transformer;
    cdf.convertTo(transformer, CV_8U);

    for (int row = 0; row < img.rows; row++)
        for (int col = 0; col < img.cols; col++) {
            uchar intensity = img.at<uchar>(row, col);
            output.at<uchar>(row, col) = transformer.at<uchar>(intensity);
        }


}

void Histogram::getMinMax(cv::Mat img, int* min, int* max)
{
    *min = 255, * max = 0;
    for (int row = 0; row < img.rows; row++)
        for (int col = 0; col < img.cols; col++) {
            if (img.at<uchar>(row, col) < *min)
                *min = (int)img.at<uchar>(row, col);
            else if (img.at<uchar>(row, col) > *max)
                *max = (int)img.at<uchar>(row, col);
        }

}

void Histogram::normalizeImg(cv::Mat& img,cv::Mat& output ,int newMin, int newMax)
{
    int min, max;
    Histogram::getMinMax(img, &min, &max);
    double scale = ((double)newMax - newMin) / ((double)max - min);
    output = cv::Mat(img.size(), img.type());
    output= (img-min)*scale + newMin;

}
void Histogram::BGR2Gray(cv::Mat& InputImage, cv::Mat& grayImage) {
    cv::Mat blue;
    cv::Mat green;
    cv::Mat red;
    Histogram::getChannels(InputImage, blue, green, red);
    grayImage = red * 0.2989 + green * 0.587 + blue * 0.114;
}

void Histogram::getChannels(cv::Mat& img, cv::Mat& blue, cv::Mat& green, cv::Mat& red ) {
    std::vector <cv::Mat> channels;
    cv::split(img, channels);
    blue = channels[0];
    green = channels[1];
    red = channels[2];
}

void Histogram::globalThreshold(cv::Mat& img, cv::Mat& output)
{
    cv::Mat pdf = cv::Mat::zeros(256, 1, CV_32F);
    cv::Mat cdf = cv::Mat::zeros(256, 1, CV_32F);
    output = cv::Mat(img.size(), img.type());
    Histogram::getHist(img, pdf, cdf);
    float pixelNum = cdf.at<float>(255);
    cdf = cdf /pixelNum;
    int bestK = 0;
    float bestVar = 0;

    cv::Mat pdf_weighted = cv::Mat(pdf.size(), pdf.type());
    pdf_weighted.at<float>(0) = 0;
    for (int i = 1; i < 256; i++) {
        pdf_weighted.at<float>(i) = pdf_weighted.at<float>(i-1) + i * pdf.at<float>(i);
    }


    for (int k = 0; k < 256; k++) {
        float weight1 = cdf.at<float>(k);
        float weight2 = 1 - cdf.at<float>(k);
        int total1 = cdf.at<float>(k) * pixelNum;
        int total2 = (cdf.at<float>(255) - cdf.at<float>(k)) * pixelNum;
        float sum1 = pdf_weighted.at<float>(k);
        float sum2 = pdf_weighted.at<float>(255)- pdf_weighted.at<float>(k);



        float mean1 = sum1 / total1;
        float mean2 = sum2 / total2;
        float betweenVar = weight2 * weight1 * (mean1 - mean2) * (mean1 - mean2);
        if (betweenVar > bestVar) {
            bestK = k;
            bestVar = betweenVar;
        }

    }
    for (int row = 0; row < img.rows; row++)
        for (int col = 0; col < img.cols; col++) {
            uchar intensity = img.at<uchar>(row, col);
            if (intensity >= bestK)
                output.at<uchar>(row, col) = 255;
            else
                output.at<uchar>(row, col) = 0;
        }
}

void Histogram::localThreshold(cv::Mat& img, cv::Mat& output, int pieceSize)
{
    /*while (img.cols % pieceSize != 0 || img.rows % pieceSize != 0)
        pieceSize--;*/
    output = cv::Mat(img.size(), img.type());
    for (int row = 0; row < img.rows; row+=pieceSize)
        for (int col = 0; col < img.cols; col += pieceSize) {

            int height = std::min(img.rows-row, pieceSize);
            int width = std::min(img.cols-col, pieceSize);

            cv::Rect area(col, row, width, height);
            cv::Mat piece = img(area);
            cv::Mat outputPiece(piece.size(), piece.type());
            Histogram::globalThreshold(piece, outputPiece);

            outputPiece.copyTo(output(area));

        }

}
