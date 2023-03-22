#include "hough.h"


void Hough::HoughLine(cv::Mat edges, std::vector<cv::Vec2d> &lines, int threshold, double angleRes)
{
    int width = edges.cols;
    int height = edges.rows;
    double rhoMax = sqrt(width * width + height * height);
    int rhoSize = 2 * round(rhoMax);
    int thetaSize = 180 * angleRes;
    cv::Mat accumulator = cv::Mat::zeros(rhoSize, thetaSize, CV_32SC1);

    std::vector<double> thetas(thetaSize);
    for (int i = 0; i < thetaSize; i++)
    {
        thetas[i] = i / angleRes;
    }
    std::cout << thetas[thetas.size() - 1];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (edges.at<uchar>(y, x) > 0)
            {
                for (int i = 0; i < thetaSize; i++)
                {
                    double rho = x * cos(thetas[i]) + y * sin(thetas[i]);
                    int rhoIndex = round(rho + rhoMax);
                    accumulator.at<int>(rhoIndex, i)++;
                }
            }
        }
    }
    for (int y = 0; y < rhoSize; y++)
    {

        for (int x = 0; x < thetaSize; x++)
        {
            if (accumulator.at<int>(y, x) >= threshold)
            {
                double rho = y - rhoMax;
                double theta = x / angleRes;
                lines.push_back(cv::Vec2d(rho, theta));
            }
        }
    }
}

void Hough::drawLines(cv::Mat &img, std::vector<cv::Vec2d> lines, cv::Scalar color)
{
    for (size_t i = 0; i < lines.size(); i++)
    {
        double rho = lines[i][0];
        double theta = lines[i][1];
        double a = cos(theta);
        double b = sin(theta);
        double x0 = a * rho;
        double y0 = b * rho;
        cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
        cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));
        cv::line(img, pt1, pt2,color, 1);
    }
}

void Hough::HoughCircle(cv::Mat edges, std::vector<cv::Vec3d> &circles, int threshold, int minRadius, int maxRadius, int radiusStep, QProgressBar* bar)
{
    std::vector<std::vector<std::vector<int>>> accumulator(edges.rows, std::vector<std::vector<int>>(edges.cols, std::vector<int>(maxRadius / radiusStep, 0)));
        for (int r = minRadius; r < maxRadius; r += radiusStep)
        {
            bar->setValue((r -(double)minRadius)/ (maxRadius - (double)minRadius) * 100);
            for (int y = 0; y < edges.rows; y++)
            {
                for (int x = 0; x < edges.cols; x++)
                {
                    if (edges.at<uchar>(y, x) == 255)
                    {
                        for (int k = 0; k < 180; k++)
                        {
                            int a = x - r * std::cos(k);
                            int b = y - r * std::sin(k);

                            if (a >= 0 && a < edges.cols && b >= 0 && b < edges.rows)
                            {
                                accumulator[b][a][r / radiusStep]++;
                            }
                        }
                    }
                }
            }
        }

        for (int y = 0; y < edges.rows; y++)
        {
            for (int x = 0; x < edges.cols; x++)
            {
                for (int r = 0; r < maxRadius / radiusStep; r++)
                {
                    if (accumulator[y][x][r] > threshold)
                    {
                        circles.push_back(cv::Vec3d(x, y, r * radiusStep));
                    }
                }
            }
        }
        bar->setValue(100);
}

void Hough::drawCircles(cv::Mat &img, std::vector<cv::Vec3d> circles, cv::Scalar color)
{
    for (int i = 0; i < circles.size(); i++)
        {
            cv::Point center = cv::Point(circles[i][0], circles[i][1]);
            cv::Scalar color(0, 255, 0);
            int size = 5;
            cv::circle(img, center, circles[i][2], cv::Scalar(0, 0, 255), 2);
            cv::line(img, cv::Point(center.x - size, center.y), cv::Point(center.x + size, center.y), color, 1);
            cv::line(img, cv::Point(center.x, center.y - size), cv::Point(center.x, center.y + size), color, 1);

    }
}

void Hough::HoughEllipse(cv::Mat img, std::vector<cv::Vec6d> &ellipses, int threshold, int minRadius, int maxRadius, QProgressBar* bar)
{
    std::vector<cv::Vec2d> nonZeroImg;
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++) {
            if (img.at<uchar>(x, y) == 255) {
                nonZeroImg.push_back(cv::Vec2d(y, x));
            }
        }
    }
    std::vector<std::thread> threadVec;
    std::mutex myMutex;
    int size = nonZeroImg.size();
    int threadN = 30;
    for (int i = 0; i < threadN; i++) {
        threadVec.push_back(std::thread(Hough::oneThreadHough, nonZeroImg, std::ref(ellipses), std::ref(myMutex), threshold, minRadius, maxRadius, (int)(((double)i / threadN) * size), (int)(((double)(i + 1) / threadN) * size)));
    }
    for (int i = 0; i < threadVec.size(); i++) {
        bar->setValue(((double)i/threadVec.size())*100 );
        if (threadVec[i].joinable())
            threadVec[i].join();
    }
}

void Hough::drawEllipse(cv::Mat &img, std::vector<cv::Vec6d> ellipses, cv::Scalar color)
{
    for (int i = 0; i < ellipses.size(); i++)
    {
        cv::Point center = cv::Point(ellipses[i][0], ellipses[i][1]);
        cv::Size axes(ellipses[i][2], ellipses[i][3]);
        cv::ellipse(img, center,axes,ellipses[i][4]*180/CV_PI, 0 , 360, color, 5);


    }
}

void Hough::oneThreadHough(std::vector<cv::Vec2d> nonZeroImg, std::vector<cv::Vec6d> &ellipses, std::mutex &myMutex, int threshold, int minRadius, int maxRadius, int start, int end)
{
    int size = nonZeroImg.size();
    double max_sqaured_b = maxRadius * maxRadius;
    std::vector<double> acc;
    for (int i = start; i < end; i++) {

        cv::Vec2d P1 = nonZeroImg[i];
        for (int j = 0; j < i; j++) {
            cv::Vec2d P2 = nonZeroImg[j];

            double dx = P1[0] - P2[0];
            double dy = P1[1] - P2[1];

            double MajorR = 0.5 * std::sqrt(dx * dx + dy * dy);
            if (MajorR > 0.5 * minRadius) {

                cv::Vec2d Pc(0.5 * (P1[0] + P2[0]), 0.5 * (P1[1] + P2[1]));
                for (int z = 0; z < size; z++) {
                    cv::Vec2d P3 = nonZeroImg[z];
                    dx = P3[0] - Pc[0];
                    dy = P3[1] - Pc[1];
                    double d = std::sqrt(dx * dx + dy * dy);
                    if (d > minRadius) {
                        dx = P3[0] - P1[0];
                        dy = P3[1] - P1[1];
                        double cos_tau = (MajorR * MajorR + d * d - dx * dx - dy * dy) / (2 * MajorR * d);
                        cos_tau *= cos_tau;
                        double k = MajorR * MajorR - d * d * cos_tau;
                        if (k > 0 && cos_tau < 1) {
                            double MinorR_squared = MajorR * MajorR * d * d * (1 - cos_tau) / k;
                            if (MinorR_squared <= max_sqaured_b)
                                acc.push_back(MinorR_squared);
                        }
                    }
                }


                if (!acc.empty()) {
                    int maxVal = -1;
                    for (int i = 0; i < acc.size(); i++)
                        if (std::round(acc[i]) > maxVal)
                            maxVal = std::round(acc[i]);

                    std::vector<int>pdf(maxVal + 1, 0);

                    for (int i = 0; i < acc.size(); i++)
                        pdf[std::round(acc[i])]++;

                    maxVal = 0;
                    int maxIdx = -1;
                    for (int i = 0; i < pdf.size(); i++)
                        if (pdf[i] > maxVal) {
                            maxVal = pdf[i];
                            maxIdx = i;
                        }

                    if (maxVal > threshold) {
                        double angle = std::atan2(P1[1] - P2[1], P1[0] - P2[0]);
                        double MinorR = std::sqrt(maxIdx);
                        if (angle != 0) {
                            //angle = CV_PI - angle;

                            if (angle > CV_PI) {
                                angle = angle - CV_PI / 2;
                                std::swap(MajorR, MinorR);
                            }
                        }
                        if (MinorR > minRadius && MajorR > minRadius && MajorR < maxRadius && MinorR < maxRadius) {

                            cv::Vec6d ellipse(Pc[0], Pc[1], MajorR, MinorR, angle, maxVal);
                            std::lock_guard<std::mutex> lock(myMutex);

                            ellipses.push_back(ellipse);
                        }

                    }

                    acc.clear();


                }

            }
        }

    }
}
