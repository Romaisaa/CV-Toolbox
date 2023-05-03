#ifndef SCENE_H
#define SCENE_H
#include "qnamespace.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <qpainter.h>
#include <vector>
#include <opencv2/core.hpp>>




class Scene : public QGraphicsScene
{

public:
    explicit Scene(bool multiPoints);
    ~Scene();
    void setRad(double rad);
    void drawCircle(bool isFilled);
    void setDrawFlag(bool flag);
    double getRad();
    QPointF getCenter();
    std::vector<cv::Point> seeds;



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QImage* image;
    QPointF center;
    double rad;
    bool drawFlag=false;
    bool multiPoints;
};
#endif // SCENE_H
