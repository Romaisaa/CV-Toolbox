#ifndef FREQUENCY_PAGE_H
#define FREQUENCY_PAGE_H
#include <CV/frequency.h>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QWidget>
#include<QLabel>
#include "CV/histogram.h"


namespace Ui {
class page3;
}

class page3 : public QWidget
{
    Q_OBJECT

public:
    explicit page3(QWidget *parent = nullptr);
    ~page3();
    QString low_filename;
    QString high_filename;


private slots:

    void on_low_freq_img_label_clicked();
    void on_high_freq_img_label_clicked();

    void on_low_res_valueChanged(int arg1);

    void on_high_res_valueChanged(int arg1);

private:
    Ui::page3 *ui;
    QLabel* low_freq_img_label;
    QLabel* high_freq_img_label;
    void updateOutput();
    cv::Mat low_img;
    cv::Mat high_img;
};

#endif // FREQUENCY_PAGE_H
