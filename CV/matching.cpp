#include "matching.h"

void matching::SSD(cv::Mat img, cv::Mat templ, cv::Rect& roi)
{
    cv::Mat output;
    int resultCols = img.cols - templ.cols + 1;
    int resultRows = img.rows - templ.rows + 1;
    output.create(resultRows, resultCols, CV_32S);

    for (int i = 0; i < resultRows; i++)
    {
        for (int j = 0; j < resultCols; j++) {

            cv::Rect roi = cv::Rect(j, i, templ.cols, templ.rows);
            cv::Mat roiImg = img(roi);

            cv::Mat resultImg = roiImg - templ;
            cv::pow(resultImg, 2, resultImg);
            cv::Scalar result = cv::sum(resultImg);
            output.at<int>(i, j) = result[0];
        }
    }

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(output, &minVal, &maxVal, &minLoc, &maxLoc);

    roi = cv::Rect(minLoc.x, minLoc.y, templ.cols, templ.rows);

}

void matching::crossCorrelation(cv::Mat img, cv::Mat templ, cv::Rect& roi)
{
    cv::Mat output;
    int resultCols = img.cols - templ.cols +1;
    int resultRows = img.rows - templ.rows +1;

    cv::Scalar templateMean;
    cv::Scalar templateStd;
    cv::meanStdDev(templ, templateMean, templateStd);


    output.create(resultRows, resultCols, CV_32FC1);

    for (int i = 0; i < resultRows; i++)
    {
        for (int j = 0; j < resultCols; j++) {

            cv::Rect roi = cv::Rect(j, i, templ.cols, templ.rows);
            cv::Mat roiImg = img(roi);
            cv::Scalar roiImgMean = cv::mean(roiImg);
            int counter = 0;
            double stdg1g2 = 0;
            double stdg1 = 0;
            for (int k = 0; k < templ.rows; k++) {
                for (int z = 0; z < templ.cols; z++) {
                    counter++;
                    stdg1g2 += (roiImg.at<uchar>(k, z) - roiImgMean[0]) * (templ.at<uchar>(k, z) - templateMean[0]);
                    stdg1 += (roiImg.at<uchar>(k, z) - roiImgMean[0]) * (roiImg.at<uchar>(k, z) - roiImgMean[0]);
                }
            }
            stdg1g2 /= counter;
            stdg1 /= counter;

            output.at<float>(i, j) = stdg1g2 / (templateStd[0] * templateStd[0] * stdg1 * stdg1);
        }

    }
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(output, &minVal, &maxVal, &minLoc, &maxLoc);

    roi = cv::Rect(maxLoc.x, maxLoc.y, templ.cols, templ.rows);


}

void matching::matchDes(cv::Mat des1, cv::Mat des2, std::vector <cv::Vec3d>& matches) {
    cv::Mat corrMatrix(des1.rows, des2.rows, CV_32F);

    for (int i = 0; i < des1.rows; i++) {
        cv::Mat des1Row = des1.row(i);
        cv::Scalar des1Mean, des1Std;
        cv::meanStdDev(des1Row, des1Mean, des1Std);
        for (int j = 0; j < des2.rows; j++) {
            cv::Mat des2Row = des2.row(j);
            cv::Scalar des2Mean = cv::mean(des2Row);
            int counter = 0;
            double des2Std = 0;
            double des1des2Std = 0;
            for (int k = 0; k < des2Row.cols; k++) {
                des2Std += (des2Row.at<float>(k) - des2Mean[0]) * (des2Row.at<float>(k) - des2Mean[0]);
                des1des2Std += (des2Row.at<float>(k) - des2Mean[0]) * (des1Row.at<float>(k) - des1Mean[0]);
                counter++;
            }
            des2Std /= counter;
            des1des2Std /= counter;
            corrMatrix.at<float>(i, j) = des1des2Std / (des2Std * des1Std[0]);
        }
    }

    for (int i = 0; i < des1.rows; i++)
    {
        float maxCorr = -1.0f;
        int maxIdx = -1;
        for (int j = 0; j < des2.rows; j++)
        {
            if (corrMatrix.at<float>(i, j) > maxCorr)
            {
                maxCorr = corrMatrix.at<float>(i, j);
                maxIdx = j;
            }

        }
        matches.push_back(cv::Vec3d(i, maxIdx, maxCorr));
}}
