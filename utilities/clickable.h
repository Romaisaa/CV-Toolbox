#ifndef CLICKABLE_H
#define CLICKABLE_H

#include <QWidget>
#include<QLabel>

class Clickable :public QLabel
    {
        Q_OBJECT
    signals:
        void clicked();
    public:
        void mousePressEvent(QMouseEvent* event);

        using QLabel::QLabel;
    };
#endif // CLICKABLE_H
