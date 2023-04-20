#include "segmentation.h"

void Segmentation:: meanShiftSegmentation(const cv::Mat& inputImage, cv::Mat& outputImage, float colorRadius, float spatialRadius, int maxIterations){
    cv::Mat labImage;
    cvtColor(inputImage, labImage, cv::COLOR_BGR2Lab);

      // Define the spatial and color radius for the mean shift algorithm


      // Iterate until convergence

      for (int i = 0; i < maxIterations; i++)
      {
          // Create a temporary image for storing the segmented image
          cv::Mat segmentedImage = cv::Mat::zeros(labImage.size(), labImage.type());

          // Loop over all pixels in the input image
          for (int y = 0; y < labImage.rows; y++)
          {
              for (int x = 0; x < labImage.cols; x++)
              {
                  // Compute the mean shift vector for the current pixel
                  cv::Vec3f currentPixel = labImage.at<cv::Vec3b>(y, x);
                  cv::Vec3f meanShiftVector = cv::Vec3f::all(0);
                  float weightSum = 0;

                  for (int j = -spatialRadius; j <= spatialRadius; j++)
                  {
                      int yIndex = y + j;
                      if (yIndex < 0 || yIndex >= labImage.rows) continue;

                      for (int i = -spatialRadius; i <= spatialRadius; i++)
                      {
                          int xIndex = x + i;
                          if (xIndex < 0 || xIndex >= labImage.cols) continue;

                          cv::Vec3f neighborPixel = labImage.at<cv::Vec3b>(yIndex, xIndex);
                          float spatialDistance = norm(cv::Vec2f(i, j));
                          float colorDistance = norm(currentPixel - neighborPixel);

                          if (spatialDistance <= spatialRadius && colorDistance <= colorRadius)
                          {
                              //Guassian density kernal
                              float weight = exp(-pow(spatialDistance, 2) / pow(spatialRadius, 2) - pow(colorDistance, 2) / pow(colorRadius, 2));
                              meanShiftVector += weight * neighborPixel;
                              weightSum += weight;
                          }
                      }
                  }

                  // Compute the mean shifted pixel value
                  cv::Vec3f meanShiftedPixel = meanShiftVector / weightSum;

                  // Assign the mean shifted pixel value to the segmented image
                  segmentedImage.at<cv::Vec3b>(y, x) = meanShiftedPixel;
              }
          }

          // Update the LAB image with the segmented image
          labImage = segmentedImage;
      }

      // Convert the segmented image back to the BGR color space
      cvtColor(labImage, outputImage, cv::COLOR_Lab2BGR);
}
