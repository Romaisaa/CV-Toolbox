#include "scene.h"

Scene::~Scene()
{
}
Scene::Scene()
{
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



void Scene::drawCircle()
{

    this->addEllipse(this->center.x()-0.5*this->rad, this->center.y()-0.5*this->rad, this->rad*1.0, this->rad*1.0,
                     QPen());

}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->drawFlag){
    QPointF point = event->scenePos();
    this->center= point;
    this->drawFlag=true;
    drawCircle();
    }
}




