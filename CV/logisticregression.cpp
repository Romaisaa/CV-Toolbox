#include "logisticregression.h"


LogisticRegression::LogisticRegression(double lr, int epochs)
{
    this->lr = lr;
    this->epochs = epochs;

}

cv::Mat LogisticRegression::sigmoid(cv::Mat z)
{
    cv::Mat result;
    cv::exp(-1*z, result);
    result = 1.0 / (1 + result);
    return result;
}

double LogisticRegression::loss(cv::Mat y_hat)
{
    cv::Mat Log1;
    cv::log(y_hat, Log1);
    cv::Mat Log2;
    cv::log(1 - y_hat, Log2);
    cv::Mat result1;
    cv::multiply(this->y, Log1, result1);
    cv::Mat result2;
    cv::multiply(1 - this->y, Log2, result2);
    cv::Scalar loss = cv::mean(result1 - result2);
    return -loss[0];
}

void LogisticRegression::gradientDescent(cv::Mat y_hat, cv::Mat& dW, double& db)
{
    double m = this->X.rows;
    cv::Mat X_t = this->X.t();
    dW = (1 / m) * X_t * (y_hat - this->y);
    db = (1 / m) * cv::sum(y_hat - this->y)[0];
}

void LogisticRegression::Create_normalize()
{
    this->means = cv::Mat(1, this->X.cols, this->X.type());
    this->std = cv::Mat(1, this->X.cols, this->X.type());
    for (int col = 0; col < this->X.cols; ++col) {
        cv::Mat column = this->X.col(col);
        cv::Scalar mean, stddev;
        cv::meanStdDev(column, mean, stddev);


        means.at<float>(col) = mean[0];
        std.at<float>(col) = stddev[0];
    }
    cv::Mat repeatedMeans, repeatedStd;
    cv::repeat(this->means, this->X.rows, 1, repeatedMeans);
    cv::repeat(this->std, this->X.rows, 1, repeatedStd);
    this->X = (this->X - repeatedMeans) / repeatedStd;

}

void LogisticRegression::apply_normalize(cv::Mat X, cv::Mat& output)
{
    cv::Mat repeatedMeans, repeatedStd;
    cv::repeat(means, X.rows, 1, repeatedMeans);
    cv::repeat(std, X.rows, 1, repeatedStd);
    output = (X - repeatedMeans) / repeatedStd;
}

void LogisticRegression::fit(cv::Mat X, cv::Mat y)
{
    this->X = X.clone();
    this->y = y.clone();
    int n = X.cols;
    this->W = cv::Mat::zeros(n, 1, X.type());
    this->b = 0;


    Create_normalize();

    for (int iter = 0; iter < epochs; iter++) {
        cv::Mat y_hat = sigmoid(this->X * this->W + this->b);

        cv::Mat dW;
        double db;

        gradientDescent(y_hat, dW, db);
        this->W -= this->lr * dW;
        this->b -= this->lr * db;

        double l = loss(sigmoid(this->X * this->W + this->b));
        losses.push_back(l);

    }
}

cv::Mat LogisticRegression::predict(cv::Mat X)
{
    cv::Mat temp_X = X.clone();
    apply_normalize(temp_X, temp_X);
    return sigmoid(temp_X * this->W + this->b);

}



