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


void kMeansDistance(double* distances, int* labels, const cv::Mat& data, const cv::Mat& centers, int N, int K, int dims, bool onlyDistance)
{
    for (int i = 0; i < N; ++i)
    {
        const float* sample = data.ptr<float>(i);
        if (onlyDistance)
        {
            const float* center = centers.ptr<float>(labels[i]);
            distances[i] = cv::hal::normL2Sqr_(sample, center, dims);
            continue;
        }
        else
        {
            int k_best = 0;
            double min_dist = DBL_MAX;

            for (int k = 0; k < K; k++)
            {
                const float* center = centers.ptr<float>(k);
                const double dist = cv::hal::normL2Sqr_(sample, center, dims);

                if (min_dist > dist)
                {
                    min_dist = dist;
                    k_best = k;
                }
            }

            distances[i] = min_dist;
            labels[i] = k_best;
        }
    }
}



static void generateRandomCenter(int dims, const cv::Vec2f* box, float* center, cv::RNG& rng)
{
    float margin = 1.f/dims;
    for (int j = 0; j < dims; j++)
        center[j] = ((float)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}



double Segmentation::generateKClusters(cv::InputArray _data, int K, cv::InputOutputArray _bestLabels,
                                        cv::TermCriteria criteria, int attempts, cv::OutputArray _centers)
{
    cv::Mat data0 = _data.getMat();
    const bool isrow = data0.rows == 1;
    const int N = isrow ? data0.cols : data0.rows;
    const int dims = (isrow ? 1 : data0.cols)*data0.channels();
    const int type = data0.depth();

    attempts = std::max(attempts, 1);

    cv::Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * sizeof(float) : static_cast<size_t>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, true);

    cv::Mat _labels, best_labels = _bestLabels.getMat();

    if (!((best_labels.cols == 1 || best_labels.rows == 1) &&
         best_labels.cols*best_labels.rows == N &&
         best_labels.type() == CV_32S &&
         best_labels.isContinuous()))
    {
        _bestLabels.create(N, 1, CV_32S);
        best_labels = _bestLabels.getMat();
    }
    _labels.create(best_labels.size(), best_labels.type());

    int* labels = _labels.ptr<int>();

    cv::Mat centers(K, dims, type), old_centers(K, dims, type), temp(1, dims, type);
    cv::AutoBuffer<int, 64> counters(K);
    cv::AutoBuffer<double, 64> dists(N);
    cv::RNG& rng = cv::theRNG();

    if (criteria.type & cv::TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    else
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    if (criteria.type & cv::TermCriteria::COUNT)
        criteria.maxCount = std::min(std::max(criteria.maxCount, 2), 100);
    else
        criteria.maxCount = 100;

    if (K == 1)
    {
        attempts = 1;
        criteria.maxCount = 2;
    }

    cv::AutoBuffer<cv::Vec2f, 64> box(dims);

    double best_compactness = DBL_MAX;
    for (int a = 0; a < attempts; a++)
    {
        double compactness = 0;

        for (int iter = 0; ;)
        {
            double max_center_shift = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            if (iter == 0 && (a > 0))
            {
                for (int k = 0; k < K; k++)
                    generateRandomCenter(dims, box.data(), centers.ptr<float>(k), rng);
            }
            else
            {
                // compute centers
                centers = cv::Scalar(0);
                for (int k = 0; k < K; k++)
                    counters[k] = 0;

                for (int i = 0; i < N; i++)
                {
                    const float* sample = data.ptr<float>(i);
                    int k = labels[i];
                    float* center = centers.ptr<float>(k);
                    for (int j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                for (int k = 0; k < K; k++)
                {
                    if (counters[k] != 0)
                        continue;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    int max_k = 0;
                    for (int k1 = 1; k1 < K; k1++)
                    {
                        if (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    double max_dist = 0;
                    int farthest_i = -1;
                    float* base_center = centers.ptr<float>(max_k);
                    float* _base_center = temp.ptr<float>(); // normalized
                    float scale = 1.f/counters[max_k];
                    for (int j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    for (int i = 0; i < N; i++)
                    {
                        if (labels[i] != max_k)
                            continue;
                        const float* sample = data.ptr<float>(i);
                        double dist = cv::hal::normL2Sqr_(sample, _base_center, dims);

                        if (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    const float* sample = data.ptr<float>(farthest_i);
                    float* cur_center = centers.ptr<float>(k);
                    for (int j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                for (int k = 0; k < K; k++)
                {
                    float* center = centers.ptr<float>(k);
                    CV_Assert( counters[k] != 0 );

                    float scale = 1.f/counters[k];
                    for (int j = 0; j < dims; j++)
                        center[j] *= scale;

                    if (iter > 0)
                    {
                        double dist = 0;
                        const float* old_center = old_centers.ptr<float>(k);
                        for (int j = 0; j < dims; j++)
                        {
                            double t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            bool isLastIter = (++iter == MAX(criteria.maxCount, 2) || max_center_shift <= criteria.epsilon);

            if (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                kMeansDistance(dists, labels, data, centers, N, K, dims, true);
                compactness = sum(cv::Mat(cv::Size(N, 1), CV_64F, &dists[0]))[0];
                break;
            }
            else
            {
                // assign labels
                kMeansDistance(dists, labels, data, centers, N, K, dims, false);
            }
        }

        if (compactness < best_compactness)
        {
            best_compactness = compactness;
            if (_centers.needed())
            {
                if (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                else
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
    }

    return best_compactness;
}

void Segmentation::kmeansSegmentation(cv::Mat inputImage, cv::Mat &outputImage, int K) {
    cv::Mat samples(inputImage.rows * inputImage.cols, inputImage.channels(), CV_32F);
    for (int y = 0; y < inputImage.rows; y++)
        for (int x = 0; x < inputImage.cols; x++)
            for (int z = 0; z < inputImage.channels(); z++)
                if (inputImage.channels() == 3) {
                    samples.at<float>(y + x * inputImage.rows, z) = inputImage.at<cv::Vec3b>(y, x)[z];
                }
                else {
                    samples.at<float>(y + x * inputImage.rows, z) = inputImage.at<uchar>(y, x);
                }

    cv::Mat labels;
    int attempts = 5;
    cv::Mat centers;
    Segmentation::generateKClusters(samples, K, labels, cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 10000, 0.0001), attempts, centers);

    cv::Mat new_image(inputImage.size(), inputImage.type());
    for (int y = 0; y < inputImage.rows; y++)
        for (int x = 0; x < inputImage.cols; x++)
        {
            int cluster_idx = labels.at<int>(y + x * inputImage.rows, 0);
            if (inputImage.channels()==3) {
                for (int i = 0; i < inputImage.channels(); i++) {
                    new_image.at<cv::Vec3b>(y, x)[i] = centers.at<float>(cluster_idx, i);
                }
            }
            else {
                new_image.at<uchar>(y, x) = centers.at<float>(cluster_idx, 0);
            }
        }
    outputImage = new_image;
}
