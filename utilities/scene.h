#ifndef SCENE_H
#define SCENE_H
#include "qnamespace.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <qpainter.h>




class Scene : public QGraphicsScene
{

public:
    explicit Scene();
    ~Scene();
    void setRad(double rad);
    void drawCircle();
    void setDrawFlag(bool flag);
    double getRad();
    QPointF getCenter();



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QImage* image;
    QPointF center;
    double rad;
    bool drawFlag=false;
};
#endif // SCENE_H
