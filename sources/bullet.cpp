#include "include/bullet.h"
#include "include/player.h"
#include "include/npc.h"
#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

Bullet::Bullet(std::string attackerName, QPointF start, QPointF end, QObject *parent)
    : QObject(parent), QGraphicsItem()
{
    this->attackerName = attackerName;
    this->setRotation(0);   // Устанавливаем начальный разворот
    this->setPos(start);    // Устанавливаем стартовую позицию
    // Определяем траекторию полёта пули
    QLineF lineToTarget(start, end);
    // Угол поворота в направлении к цели
    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
    if (lineToTarget.dy() < 0)
        angleToTarget = TwoPi - angleToTarget;
    angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

    /* Разворачиваем пули по траектории
     * */
    if (angleToTarget >= 0 && angleToTarget < Pi) {
        /// Rotate left
        setRotation(rotation() - angleToTarget * 180 /Pi);
    } else if (angleToTarget <= TwoPi && angleToTarget > Pi) {
        /// Rotate right
        setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
    }
    // Инициализируем полётный таймер
    timerBullet = new QTimer();
    // И подключаем его к слоту для обработки полёта пули
    connect(timerBullet, &QTimer::timeout, this, &Bullet::slotTimerBullet);
    timerBullet->start(10);

}

Bullet::~Bullet()
{

}

QRectF Bullet::boundingRect() const
{
    return QRectF(0,0,3,8);
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::darkYellow);
    painter->setBrush(Qt::darkYellow);
    painter->drawRect(0,0,3,8);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Bullet::slotTimerBullet()
{
    setPos(mapToParent(0, -10));

    if(!scene()->collidingItems(this).isEmpty()){

        QList<QGraphicsItem*> collige=collidingItems();
        for(int i=0;i<collige.size();++i){
            if(typeid(*collige[i])==typeid (Player)){

                Player *one = dynamic_cast<Player*>(collige[i]);
                if(one->getValue().name!=attackerName){
                    one->hit(10);
                }
            }
            else if(typeid(*collige[i])==typeid (Npc)) {

                Npc *one = dynamic_cast<Npc*>(collige[i]);
                if(one->getValue().name!=attackerName){
                    one->setAttackerName(one->getValue());
                    one->hit(10, this->attackerName);
                }
                //one->reAgressive();
            }
        }
        this->deleteLater();    // Уничтожаем пулю

    }
}

std::string Bullet::getAttackerName() {

    return attackerName;
}


