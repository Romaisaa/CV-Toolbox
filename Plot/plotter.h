#ifndef PLOTTER_H
#define PLOTTER_H
#include <QVector>
#include "utilities/qcustomplot.h"
class plotter
{
public:
    static void plotHist(QCustomPlot* graph, QVector<double>& data,  Qt::GlobalColor color);
    static void plotROC(QCustomPlot* graph,std::pair<std::vector<float>,std::vector<float>>ROC);
    static void plotAllROC(QCustomPlot* graph,std::vector<std::pair<std::vector<float>,std::vector<float>>> ROC, std::unordered_map<int, std::string> LabelToPerson);
};

#endif // PLOTTER_H
