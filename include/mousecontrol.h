#ifndef MouseControl_H
#define MouseControl_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
/*!
    \brief Класс для отслежки перемещения мыши.

    Данный класс нужен для управления игрока.
*/
class MouseControl : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MouseControl(QObject *parent = nullptr);
    ~MouseControl();

signals:
    /*!
    Сигнал для передачи координат положения курсора мыши.
    */
    void signalTargetCoordinate(QPointF point);

    /*!
    Сигнал на стрельбу.
    */
    void signalShot(bool shot);

public slots:

private:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};


#endif // MouseControl_H
