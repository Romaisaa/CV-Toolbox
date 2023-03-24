#ifndef PLOTTER_H
#define PLOTTER_H
#include <QVector>
#include "utilities/qcustomplot.h"
class plotter
{
public:
    static void plotHist(QCustomPlot* graph, QVector<double>& data,  Qt::GlobalColor color);
};

#endif // PLOTTER_H
