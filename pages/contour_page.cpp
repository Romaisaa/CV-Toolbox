#include "contour_page.h"
#include "ui_page5.h"

page5::page5(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page5)
{
    ui->setupUi(this);
}

page5::~page5()
{
    delete ui;
}

void page5::on_window_size_editingFinished()
{
    int val = ui->window_size->value();
        if(val % 2 == 0)
        ui->window_size->setValue(val-1);
}


void page5::on_window_size_valueChanged(int arg1)
{
    int val = ui->window_size->value();
        if(val % 2 == 0)
        ui->window_size->setValue(val-1);
}


void page5::on_upload_btn_clicked()
{
    Img= QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    QImage image(Img);
    scene = new Scene(false);
    scene->setRad(ui->raduis->value());
    updateImage();

}

void page5::updateImage(){
    if (!Img.isEmpty()) {
        QImage image(Img);
        scene->addPixmap(QPixmap::fromImage(image));
        ui->graphicsView->setScene(scene);
        ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    }
}


void page5::on_raduis_valueChanged(int arg1)
{
    scene->setRad(arg1);
    updateImage();
    scene->drawCircle(false);

}


void page5::on_clear_clicked()
{
    updateImage();
    scene->setDrawFlag(false);
}
void page5::resizeEvent(QResizeEvent *e)
{
  updateImage();
  //scene->setDrawFlag(false);
  //scene->drawCircle();
}

void page5::on_generate_btn_clicked()
{

    Mat imageCv= cv::imread(Img.toStdString(),IMREAD_COLOR);
    Mat output;
    Point center=  Point_<double>(scene->getCenter().x(), scene->getCenter().y());

    vector<Point> s_vector =snake::active_contour(imageCv,output,center,ui->raduis->value(),ui->numItr->value(),ui->num_points->value(),
                                                  ui->window_size->value(),ui->alpha->value(),ui->beta->value(),ui->gamma->value());

    double area = snake::calculate_contour_area(s_vector);
    double perimeter=snake::calculate_contour_perimeter(s_vector);
    ui->area->setMaximum(area+10.0);
    ui->perimeter->setMaximum((perimeter+10.0));
    ui->area->setValue(area);
    ui->perimeter->setValue(perimeter);

    QImage qimage(output.data, output.cols, output.rows,output.step,QImage::Format_BGR888);
    QPixmap* image = new QPixmap(QPixmap::fromImage(qimage));
    int w = ui->output_label->width();
    int h = ui->output_label->height();
    ui->output_label->setPixmap(image->scaled(w,h,Qt::KeepAspectRatio));
}

