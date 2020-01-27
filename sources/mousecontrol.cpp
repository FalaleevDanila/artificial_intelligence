#include "include/mousecontrol.h"
#include <QGraphicsScene>


MouseControl::MouseControl(QObject *parent) :  QGraphicsScene()
{
    Q_UNUSED(parent);
}

MouseControl::~MouseControl()
{

}

void MouseControl::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalTargetCoordinate(event->scenePos());
}

void MouseControl::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   emit signalShot(true); // Когда клавиша мыши нажата - стрелять
   Q_UNUSED(event);
}

void MouseControl::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalShot(false); // Когда клавишу мыши отпустили - стрелять нельзя
    Q_UNUSED(event);
}
