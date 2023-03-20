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
