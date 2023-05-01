#ifndef PAGE10_H
#define PAGE10_H

#include <QWidget>
#include<CV/segmentation.h>
#include<QFileDialog>
#include<QLabel>

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
    void on_input_image_label_clicked();

    void on_applyBtn_clicked();

    void on_type_comboBox_currentIndexChanged(int index);

private:
    Ui::Page10 *ui;
    QPixmap image;
    cv::Mat img;
    QString fileName;
    void rescaleImg(QLabel* imgSlot, QPixmap& image);

};

#endif // PAGE10_H
