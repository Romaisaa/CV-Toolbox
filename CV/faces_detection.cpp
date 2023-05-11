#include "faces_detection.h"
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

void faces_detection::detect_faces(cv::Mat& inputImage, cv::Mat& outputImage, bool isCropped)
{
    // Load the cascade classifier
    cv::CascadeClassifier face_cascade;
    face_cascade.load("C://Users/kamel/OneDrive/Documents/GitKraken/CV-Toolbox/models/haarcascade_frontalface_default.xml");

    cv::Mat gray;
    cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);

    // Detect faces in the grayscale image
    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.1, 4);

    // Draw rectangles around the detected faces
    for (int i = 0; i < faces.size(); i++)
    {
        rectangle(inputImage, faces[i], cv::Scalar(0, 255, 0), 2);
    }

    if(isCropped){
        cropImage(inputImage, outputImage, faces[0]);
    }else{
        outputImage = inputImage;
    }
}


void faces_detection::cropImage(cv::Mat inputImage, cv::Mat& outputImage, cv::Rect faceRect) {
    int targetWidth = 109;
    int targetHeight = 109;

    int widthDifference = targetWidth - faceRect.width;
    int heightDifference = targetHeight - faceRect.height;

    int topLeftX = faceRect.x;
    int topLeftY = faceRect.y;
    int bottomRightX = faceRect.x + faceRect.width;
    int bottomRightY = faceRect.y + faceRect.height;

    if (widthDifference > 0) {
        int leftOffset = widthDifference / 2;
        int rightOffset = widthDifference - leftOffset;
        topLeftX -= leftOffset;
        bottomRightX += rightOffset;
    }

    if (topLeftX < 0) {
        bottomRightX -= topLeftX;
        topLeftX = 0;
    } else if (bottomRightX > inputImage.cols) {
        int rightOverflow = bottomRightX - inputImage.cols;
        topLeftX -= rightOverflow;
        bottomRightX = inputImage.cols;
        if (topLeftX < 0) {
            bottomRightX -= topLeftX;
            topLeftX = 0;
        }
    }

    if (heightDifference > 0) {
        int topOffset = heightDifference / 2;
        int bottomOffset = heightDifference - topOffset;
        topLeftY -= topOffset;
        bottomRightY += bottomOffset;
    }

    if (topLeftY < 0) {
        bottomRightY -= topLeftY;
        topLeftY = 0;
    } else if (bottomRightY > inputImage.rows) {
        int bottomOverflow = bottomRightY - inputImage.rows;
        topLeftY -= bottomOverflow;
        bottomRightY = inputImage.rows;
        if (topLeftY < 0) {
            bottomRightY -= topLeftY;
            topLeftY = 0;
        }
    }

    cv::Rect croppedRect(topLeftX, topLeftY, bottomRightX - topLeftX, bottomRightY - topLeftY);
    cv::Mat croppedImage = inputImage(croppedRect);

    outputImage = croppedImage.clone();
}


