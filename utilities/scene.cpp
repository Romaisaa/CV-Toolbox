#include "scene.h"

Scene::~Scene()
{
}
Scene::Scene(bool multiPoints)
{
    this->multiPoints = multiPoints;
}

void Scene::setRad(double rad)
{
    this->rad=rad;
}
void Scene::setDrawFlag(bool flag)
{
    this->drawFlag=flag;

}

double Scene::getRad()
{
    return this->rad;

}

QPointF Scene::getCenter()
{
    return this->center;
}



void Scene::drawCircle(bool isFilled)
{
    QPen pen;
    if(isFilled){
        QBrush brush(Qt::red);
        this->addEllipse(this->center.x()-this->rad, this->center.y()-this->rad, this->rad*2.0, this->rad*2.0, pen, brush);
    }else{
        this->addEllipse(this->center.x()-this->rad, this->center.y()-this->rad, this->rad*2.0, this->rad*2.0, pen);
    }
    // This Line for region growing segmentation only
    this->seeds.push_back(cv::Point(this->center.y(), this->center.x()));
}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->drawFlag){
        QPointF point = event->scenePos();
        this->center= point;
        if(!this->multiPoints){
            this->drawFlag=true;
            drawCircle(false);
        }else{
            drawCircle(true);
        }
    }
}




