#ifndef PLOTTER_H
#define PLOTTER_H
#include "qcustomplot.h"
#include <QVector>

class plotter
{
public:
    static void plotHist(QCustomPlot* graph, QVector<double>& data,  Qt::GlobalColor color);
};

#endif // PLOTTER_H
