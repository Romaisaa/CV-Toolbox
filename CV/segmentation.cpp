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
void Segmentation::closetClusters(std::vector<cv::Vec3b> clusters, std::vector<cv::Vec3d>& minDistances, std::mutex& myMutex, int start_i, int end_i) {
    float min_dist = FLT_MAX;
    int min_i, min_j;
    for (int i = start_i; i < end_i; i++) {

        for (int j = 0; j < clusters.size(); j++) {
            if (i == j) continue;
            float dist = Segmentation::distance(clusters[i], clusters[j]);
            if (dist < min_dist) {
                min_dist = dist;
                min_i = i;
                min_j = j;
            }
        }
    }
    cv::Vec3d distance(min_i, min_j, min_dist);
    std::lock_guard<std::mutex> lock(myMutex);
    minDistances.push_back(distance);

}
void Segmentation::agglomarativeSegmentation(cv::Mat& input, cv::Mat& segmented_img, int num_clusters, float dist_threshold,QProgressBar * progress_bar) {
    // Initialize the clusters
    std::vector<cv::Vec3b> clusters;
    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            clusters.push_back(input.at<cv::Vec3b>(i, j));
        }
    }
    int steps = clusters.size()/75;
    double n_clusters=clusters.size();

    // Perform agglomerative clustering baseed on stopping criteria
    while (clusters.size() > num_clusters) {
//         std::cout << "n_c" << clusters.size() << std::endl;
        qDebug()<<"n_c"<<clusters.size()<<Qt::endl;
        progress_bar->setValue((int)((1.0-(clusters.size()/n_clusters-num_clusters))*100));
         // Find the two closest clusters
        std::vector<std::thread> threadVec;
        std::vector<cv::Vec3d> minDistances;
        std::mutex myMutex;
        if (clusters.size() < steps) {
            Segmentation::closetClusters(clusters, minDistances, myMutex, 0, clusters.size());
        }
        else {
            for (int i = 0; i <= clusters.size() - steps; i = i + steps) {
                threadVec.push_back(std::thread(Segmentation::closetClusters, clusters, std::ref(minDistances), std::ref(myMutex), i, i + steps));

            }
            for (int i = 0; i < threadVec.size(); i++) {
                if (threadVec[i].joinable())
                    threadVec[i].join();
            }
            threadVec.clear();
        }
        int min_i; int min_j;
        for (int i = 0; i < minDistances.size();i++) {
            float min_dist = minDistances[0][2];
            if (minDistances[i][2] <= min_dist) {
                min_i = minDistances[i][0];
                min_j = minDistances[i][1];

            }
        }
        // Merge the two clusters
        clusters[min_i] = merge(clusters[min_i], clusters[min_j]);
        clusters.erase(clusters.begin() + min_j);
    }

    // Assign labels to the pixels
    cv::Mat labels(input.size(), CV_32S);
    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            int min_cluster = 0;
            float min_dist = FLT_MAX;
            for (int k = 0; k < clusters.size(); k++) {
                float dist = distance(input.at<cv::Vec3b>(i, j), clusters[k]);
                if (dist < min_dist) {
                    min_dist = dist;
                    min_cluster = k;
                }
            }
            labels.at<int>(i, j) = min_cluster;
        }
    }

    // Visualize the segmentation

    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            cv::Vec3b color = clusters[labels.at<int>(i, j)];
            segmented_img.at<cv::Vec3b>(i, j) = color;
        }
    }
}
// Define distance function for RGB color space
float Segmentation::distance(cv::Vec3b a, cv::Vec3b b) {
    return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2) + pow(a[2] - b[2], 2));
}

// Define merge function for merging two clusters
cv::Vec3b Segmentation:: merge(cv::Vec3b a, cv::Vec3b y) {
    int r = (a[0] + y[0]) / 2;
    int g = (a[1] + y[1]) / 2;
    int b = (a[2] + y[2]) / 2;
    return cv::Vec3b(static_cast<uchar>(r), static_cast<uchar>(g), static_cast<uchar>(b));

}
