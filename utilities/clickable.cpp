#include "clickable.h"
#include "qevent.h"

void Clickable::mousePressEvent(QMouseEvent* event)
{
    emit clicked();

}
