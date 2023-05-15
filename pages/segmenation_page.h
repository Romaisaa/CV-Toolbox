#ifndef SEGMENATION_PAGE_H
#define SEGMENATION_PAGE_H

#include <QWidget>
#include<CV/segmentation.h>
#include<QFileDialog>
#include<QLabel>
#include "utilities/scene.h"
#include <QGraphicsScene>

namespace Ui {
class Page10;
}

class Page10 : public QWidget
{
    Q_OBJECT

public:
    explicit Page10(QWidget *parent = nullptr);
    ~Page10();

private slots:
    void on_applyBtn_clicked();

    void on_type_comboBox_currentIndexChanged(int index);

    void on_inputLabel_clicked();

    void on_uploadImage_clicked();

private:
    Ui::Page10 *ui;
    QPixmap image;
    cv::Mat img;
    QString fileName;
    QString Img;
    Scene* scene;
    void rescaleImg(QLabel* imgSlot, QPixmap& image);
    void updateImage();
};

#endif // SEGMENATION_PAGE_H
