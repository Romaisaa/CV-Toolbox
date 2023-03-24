#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <qlabel.h>
#include <unistd.h>
using namespace cv;
using namespace std;

class snake
{
public:
    snake();
    static vector<Point> active_contour(Mat inputimage, Mat &outputimage,
                                Point center, int radius,
                                int numOfIterations, int numOfPoints,
                                int window_size, double alpha, double beta, double gamma,QLabel * label);

    static double calculate_contour_area(vector<Point> snake_points);
    static double calculate_contour_perimeter(vector<Point> snake_points);

private:
    static vector<Point> initialize_contours(Point center, int radius, int numberOfPoints);

    static double points_distance(int x1, int y1, int x2, int y2);

    static double calculate_internal_energy(Point point, Point previousPoint, Point nextPoint, double alpha);
    static double calculate_external_energy(Mat image, Point point, double beta);
    static double calculate_gradients(Point point, Point previousPoint, double gamma);
    static double calculate_point_enegy(Mat image, Point point, Point previousPoint, Point nextPoint, double alpha, double beta, double gamma);

    static void snake_operation(Mat image, vector<Point>& curve, int window_size, double alpha, double beta, double gamma);
    static void draw_contours(Mat image, Mat &outputimage, vector<Point> snake_points);
};

#endif // SNAKE_H
