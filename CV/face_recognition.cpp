#include "face_recognition.h"

void face_recognition::setModel(std::string filePath, cv::Mat images, std::vector<std::string> labels)
{
    cv::FileStorage fs(filePath, cv::FileStorage::READ);
    fs["eigenValues"] >> eigenValues;
    fs["eigenVectors"] >> eigenVectors;
    fs.release();
    this->labels = labels;
    this->images = images;
    cv::Mat tempVectors = eigenVectors.rowRange(0, this->n_component);
    multiplyEigen(tempVectors, images, this->transformedImages);

}

void face_recognition::train(cv::Mat images, std::vector<std::string> labels)
{
    performPCA(images);
    this->labels = labels;
    this->images = images;
    cv::Mat tempVectors = eigenVectors.rowRange(0, this->n_component);
    multiplyEigen(tempVectors, images, this->transformedImages);

}

void face_recognition::setNComponent(int n_component)
{
    this->n_component = n_component;
    cv::Mat tempVectors = eigenVectors.rowRange(0, this->n_component);
    multiplyEigen(tempVectors, this->images, this->transformedImages);
}

std::string face_recognition::getPerson(cv::Mat image)
{

    cv::Mat reshapedImage = image.reshape(1, 1);
    reshapedImage.convertTo(reshapedImage, CV_32F);
    cv::Mat tempEigen = eigenVectors.rowRange(0, n_component);
    cv::Mat output;
    multiplyEigen(tempEigen, reshapedImage, output);
    return labels[getNearest(this->transformedImages, output)];


}

void face_recognition:: performPCA(cv::Mat& dataPoints)
{
    cv::Mat mean;
    cv::reduce(dataPoints, mean, 0, cv::REDUCE_AVG);
    for (int i = 0; i < dataPoints.rows; i++)
    {
        dataPoints.row(i) = dataPoints.row(i) - mean;
    }
    cv::Mat covariance;
    cv::mulTransposed(dataPoints, covariance, true);
    covariance = covariance / (dataPoints.rows - 1);
    cv::eigen(covariance, this->eigenValues, this->eigenVectors);



}

void face_recognition:: multiplyEigen(cv::Mat& eigenvectors, cv::Mat& images, cv::Mat& result)
{

    result = images * eigenvectors.t();
}

int face_recognition::getNearest(cv::Mat images, cv::Mat image) {
    cv::Mat distances= cv::Mat(images.rows, 1, CV_32F);

    for (int row = 0; row < images.rows; row++) {
        double distance = 0;
        for (int col = 0; col < images.cols; col++) {
            distance += (images.at<float>(row, col) - image.at<float>(col)) * (images.at<float>(row, col) - image.at<float>(col));
        }
        distances.at<float>(row) = distance;
    }

    double minValue;
    cv::Point minLocation;
    cv::minMaxLoc(distances, &minValue, nullptr, &minLocation, nullptr);
    return minLocation.y;
}
