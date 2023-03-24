#ifndef PAGE5_H
#define PAGE5_H

#include <QWidget>
#include "Scene.h"
#include <QGraphicsScene>
#include <QFileDialog>
#include "CV/snake.h"

namespace Ui {
class page5;
}

class page5 : public QWidget
{
    Q_OBJECT

public:
    explicit page5(QWidget *parent = nullptr);
    ~page5();

private slots:
    void on_window_size_editingFinished();

    void on_window_size_valueChanged(int arg1);

    void on_upload_btn_clicked();

    void on_raduis_valueChanged(int arg1);

    void on_clear_clicked();

    void on_generate_btn_clicked();

protected:
    void resizeEvent(QResizeEvent* e);
private:
    Ui::page5 *ui;
    QString Img;
    Scene* scene;
    void updateImage();

};

#endif // PAGE5_H
