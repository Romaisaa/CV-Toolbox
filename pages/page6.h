#ifndef PAGE6_H
#define PAGE6_H

#include <QWidget>
#include <opencv2/core.hpp>
#include <QFileDialog>
#include <CV/histogram.h>
#include <CV/harris.h>

namespace Ui {
class Page6;
}

class Page6 : public QWidget
{
    Q_OBJECT

public:
    explicit Page6(QWidget *parent = nullptr);
    ~Page6();

private slots:
    void on_input_label_clicked();
    void updateOutput();



    void on_ksize_editingFinished();

    void on_threshold_valueChanged(double arg1);

    void on_ksize_valueChanged(int arg1);

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Page6 *ui;
    cv::Mat grayImg;
    QString img_filename;
    bool operator_type=true;

};

#endif // PAGE6_H
