#ifndef HISTOGRAM_PAGE_H
#define HISTOGRAM_PAGE_H

#include <QWidget>
#include <QFileDialog>
#include <QString>
#include <QPixmap>
#include <QVector>
#include <CV/histogram.h>
#include <Plot/plotter.h>
#include<QImage>
#include <opencv2/core.hpp>


namespace Ui {
class page2;
}


class page2 : public QWidget
{
    Q_OBJECT

public:

    explicit page2(QWidget *parent = nullptr);
    ~page2();

private slots:
    void uploadImage();
    void on_ModeCombo1_currentIndexChanged(int index);

    void on_minNormVal_valueChanged(int arg1);

    void on_maxNormVal_valueChanged(int arg1);

    void on_OperationBox_currentIndexChanged(int index);


    void on_thres_combox_currentIndexChanged(int index);

    void on_pieceSize_spiner_valueChanged(int arg1);

    void on_image5_clicked();

    void on_image3_clicked();

    void on_image_clicked();

private:
    QPixmap imageGray;
    QPixmap image;
    cv::Mat img;
    cv::Mat imgGray;
    bool hasFile=false;
    bool normalizeMode=false;
    QString fileName;
    Ui::page2 *ui;
    void updateOutput();
    void rescaleImg(QLabel* imgSlot, QPixmap& image);
};

#endif // HISTOGRAM_PAGE_H
