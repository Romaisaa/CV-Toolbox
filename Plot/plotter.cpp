#include "plotter.h"


void plotter::plotHist(QCustomPlot* graph, QVector<double> &data, Qt::GlobalColor color)
{
    graph->clearPlottables();
    QVector<double> ticks(256);
    for (int i = 0; i < 256; i++) {
        ticks[i] = i;
    }
    QCPBars *bars = new QCPBars(graph->xAxis, graph->yAxis);
    QBrush redBrush(color);
    bars->setBrush(redBrush);
    QPen redPen(color);
    bars->setPen(redPen);
    graph->addGraph();
    bars->setData(ticks,data);
    graph->xAxis->setRange(0, 255);
    graph->yAxis->setVisible(false);
    graph->yAxis->setTickLabels(false);


    graph->yAxis->setRange(0, *std::max_element(data.begin(), data.end()));

    // rescale the axes to fit the data
    graph->rescaleAxes();
    graph->replot();
}

void plotter::plotROC(QCustomPlot* graph,std::pair<std::vector<float>,std::vector<float>>ROC)
{
    QVector<double> FPRs;
    for (const auto& value : ROC.first) {
        FPRs.append(value);
    }

    QVector<double> TPRs;
    for (const auto& value : ROC.second) {
        TPRs.append(value);
    }

    graph->clearPlottables();

    QCPGraph *plot = new QCPGraph(graph->xAxis, graph->yAxis);
    QPen pen(Qt::blue);
    plot->setPen(pen);
    graph->addGraph();
    plot->setData(FPRs,TPRs);

    graph->yAxis->setRange(0,1);
    graph->xAxis->setRange(0,1);
    graph->xAxis->setLabel("False positive Rate");
    graph->yAxis->setLabel("True Positive Rate");
    QCPItemLine *line = new QCPItemLine(graph);
    line->start->setCoords(0,0);
    line->end->setCoords(1,1);
    QPen pen2(Qt::red);
    pen2.setStyle(Qt::DashLine);
    line->setPen(pen2);



    graph->replot();
}

void plotter::plotAllROC(QCustomPlot *graph, std::vector<std::pair<std::vector<float>, std::vector<float> > > ROC, std::unordered_map<int, std::string> LabelToPerson)
{
    graph->clearPlottables();
    for(int i = 0; i < ROC.size(); i++){
    QVector<double> FPRs;
    for (const auto& value : ROC[i].first) {
        FPRs.append(value);
    }

    QVector<double> TPRs;
    for (const auto& value : ROC[i].second) {
        TPRs.append(value);
    }

    int red = QRandomGenerator::global()->bounded(256);
    int green = QRandomGenerator::global()->bounded(256);
    int blue = QRandomGenerator::global()->bounded(256);

    QColor plotColor =  QColor(red, green, blue);

    QCPGraph *plot = new QCPGraph(graph->xAxis, graph->yAxis);
    QPen pen(plotColor);
    plot->setPen(pen);
    plot->setData(FPRs,TPRs);
    plot->setName(QString::fromStdString(LabelToPerson[i]));
}


    graph->yAxis->setRange(0,1);
    graph->xAxis->setRange(0,1);
    graph->xAxis->setLabel("False positive Rate");
    graph->yAxis->setLabel("True Positive Rate");
    QCPItemLine *line = new QCPItemLine(graph);
    line->start->setCoords(0,0);
    line->end->setCoords(1,1);
    QPen pen2(Qt::red);
    pen2.setStyle(Qt::DashLine);
    line->setPen(pen2);
    graph->legend->setVisible(true);
    graph->replot();
}
