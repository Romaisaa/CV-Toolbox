#include "thresholding.h"

Thresholding::Thresholding()
{

}

void Thresholding::getMinMax(cv::Mat img, int* min, int* max)
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


void Thresholding::getHist(cv::Mat& img, cv::Mat& pdf, cv::Mat& cdf)
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




void Thresholding::otsuThresholding(cv::Mat& img, cv::Mat& output)
{
    cv::Mat pdf = cv::Mat::zeros(256, 1, CV_32F);
    cv::Mat cdf = cv::Mat::zeros(256, 1, CV_32F);
    output = cv::Mat(img.size(), img.type());
    Thresholding::getHist(img, pdf, cdf);
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
        int total2 = (1 - cdf.at<float>(k)) * pixelNum;
        float sum1 = pdf_weighted.at<float>(k);
        float sum2 = pdf_weighted.at<float>(255) - pdf_weighted.at<float>(k);



        float mean1 = sum1 / total1;
        float mean2 = sum2 / total2;
        float betweenVar = weight2 * weight1 * (mean1 - mean2) * (mean1 - mean2);
        if (betweenVar > bestVar) {
            bestK = k;
            bestVar = betweenVar;
        }

    }
    std::cout << bestK << std::endl;
    for (int row = 0; row < img.rows; row++)
        for (int col = 0; col < img.cols; col++) {
            uchar intensity = img.at<uchar>(row, col);
            if (intensity >= bestK)
                output.at<uchar>(row, col) = 255;
            else
                output.at<uchar>(row, col) = 0;
        }


}




void Thresholding::optimalThresholding(cv::Mat& img, cv::Mat& output) {

    int oldT = 20;
    int newT = 0;
    cv::Mat pdf = cv::Mat::zeros(256, 1, CV_32F);
    cv::Mat cdf = cv::Mat::zeros(256, 1, CV_32F);
    output = cv::Mat(img.size(), img.type());
    Thresholding::getHist(img, pdf, cdf);

    while (true) {
        int sum1 = 0;
        int sum2 = 0;
        int total1 = 0;
        int total2 = 0;

        for (int i = 0; i < 256; i++) {
            if (i > oldT) {
                sum1 += i * pdf.at<float>(i);
                total1 += pdf.at<float>(i);
            }
            else {
                sum2 += i * pdf.at<float>(i);
                total2 += pdf.at<float>(i);
            }
        }

        int m1 = sum1 / total1;
        int m2 = sum2 / total2;
        newT = (m1 + m2) / 2;
        if (newT == oldT)
            break;

        oldT = newT;
    }

    for (int row = 0; row < img.rows; row++)
        for (int col = 0; col < img.cols; col++) {
            uchar intensity = img.at<uchar>(row, col);
            if (intensity >= newT)
                output.at<uchar>(row, col) = 255;
            else
                output.at<uchar>(row, col) = 0;
        }

}

void Thresholding::multiSpectralThreshold(cv::Mat& img, cv::Mat& output) {

    std::vector<cv::Mat> inputChannels;

    cv::split(img, inputChannels);

    std::vector<cv::Mat> outputChannels(inputChannels.size());

    for (int i = 0; i < inputChannels.size(); i++) {
        Thresholding::otsuThresholding(inputChannels[i], outputChannels[i]);
    }

    cv::merge(outputChannels, output);
}

void Thresholding::localThreshold(cv::Mat& img, cv::Mat& output, int pieceSize)
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
            Thresholding::multiLevelOtsu(piece, outputPiece, 2);

            outputPiece.copyTo(output(area));

        }

}

void Thresholding::multiLevelOtsu(cv::Mat img, cv::Mat& output, int M)
{

    int L = 256;
    M = pow(2, M + 2);
    int N = L / M;
    std::vector<int> valleys;
    std::vector<int> histogram;
    std::vector<double> norm_hist;
    std::vector<int> thresholds = { 0 };


    get_histogram(img, histogram);
    normalize_histogram(histogram, norm_hist, M, L);
    find_valleys(norm_hist, valleys);
    threshold_valley_regions(histogram, thresholds, valleys, N);
    output = cv::Mat(img.size(), img.type());
    for (int row = 0; row < img.rows; row++)
        for (int col = 0; col < img.cols; col++) {
            uchar intensity = img.at<uchar>(row, col);
            int i = 0;
            while (i < thresholds.size()) {
                if (intensity < thresholds[i])
                    break;
                i++;
                //cout << i << " " << (int)intensity << " " << thresholds[i]<<endl;
            }
            if (i == thresholds.size())
                i--;

            output.at<uchar>(row, col) = 255 * (((double)i - 1) / (thresholds.size() - 1));
        }

}

void Thresholding::get_histogram(cv::Mat origin, std::vector<int>& histogram)
{
    histogram = std::vector<int>(256);

    for (int i = 0; i < origin.rows; i++) {
        for (int j = 0; j < origin.cols; j++)
        {
            int val = origin.at<uchar>(i, j);
            histogram[val]++;
        }
    }

}


void Thresholding::normalize_histogram(std::vector<int> histogram, std::vector<double>& norm_hist, int M, int L)
{
    norm_hist = std::vector<double>(M);
    int N = L / M;
    double max;

    for (int i = 0; i < L; i++) {
        norm_hist[i / N] += histogram[i];
    }

    max = *max_element(norm_hist.begin(), norm_hist.end());

    for (int i = 0; i < norm_hist.size(); i++) {
        norm_hist[i] = norm_hist[i] / max * 100.0;
    }

}


void Thresholding::find_valleys(std::vector<double> norm_hist, std::vector<int>& valleys)
{
    std::vector<double> probs(norm_hist.size());

    for (int i = 1; i < norm_hist.size() - 1; i++) {
        if (norm_hist[i] > norm_hist[i - 1] || norm_hist[i] > norm_hist[i + 1]) probs[i] = 0.0;
        else if (norm_hist[i] < norm_hist[i - 1] && norm_hist[i] == norm_hist[i + 1]) probs[i] = 0.25;
        else if (norm_hist[i] == norm_hist[i - 1] && norm_hist[i] < norm_hist[i + 1]) probs[i] = 0.75;
        else if (norm_hist[i] < norm_hist[i - 1] && norm_hist[i] < norm_hist[i + 1]) probs[i] = 1.0;
        else if (norm_hist[i] == norm_hist[i - 1] && norm_hist[i] == norm_hist[i + 1]) probs[i] = probs[i - 1];
    }

    std::vector<int> crobs(probs.size());

    for (int i = probs.size() - 2; i > 0; i--)
    {
        if (probs[i] != 0) {
            if (probs[i - 1] + probs[i] + probs[i + 1] >= 1.0) crobs[i] = 1;
            else crobs[i] = 0;
        }
        else crobs[i] = 0;
    }

    for (int i = 0; i < crobs.size(); i++) {
        if (crobs[i] > 0) valleys.push_back(i);
    }

}


void Thresholding::threshold_valley_regions(std::vector<int> histogram, std::vector<int>& thresholds, std::vector<int> valleys, int N) {

    for (int i = 0; i < valleys.size(); i++) {
        std::vector<int> temp_hist;
        int start_pos = (valleys[i] * N) - N;
        int end_pos = (valleys[i] + 2) * N;

        for (int idx = start_pos; idx < end_pos; idx++) {
            temp_hist.push_back(histogram[idx]);
        }

        thresholds.push_back(get_thresholds(temp_hist) + start_pos);
    }

}


int Thresholding::get_thresholds(std::vector<int> histogram) {
    int num_bins = histogram.size();
    int total = 0;
    int sum_total = 0;

    for (int i = 0; i < num_bins; i++) {
        total += histogram[i];
        sum_total += i * histogram[i];
    }

    double weight_background = 0.0;
    double sum_background = 0.0;

    int optimum_value = 0;
    double maximum = DBL_MIN;

    for (int i = 0; i < num_bins; i++) {
        weight_background += histogram[i];

        if (weight_background == 0) continue;

        int weight_foreground = total - weight_background;

        if (weight_foreground == 0) break;

        sum_background += i * (double)histogram[i];
        double mean_foreground = (sum_total - sum_background) / weight_foreground;
        double mean_background = sum_background / weight_background;

        double inter_class_variance = weight_background * weight_foreground * pow(mean_background - mean_foreground, 2);

        if (inter_class_variance > maximum) {
            optimum_value = i;
            maximum = inter_class_variance;
        }
    }

    return optimum_value;
}
