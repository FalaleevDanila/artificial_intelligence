#include "include/player.h"
#include "iostream"
#include <math.h>
#include "include/bullet.h"
#include "collising.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
static const double ZERO = 0;

static qreal normalizeAngle(qreal angle)
{
    while (angle < ZERO)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}
void Player::slotTarget(QPointF point)
{
    // Определяем расстояние до цели
    target = point;
    QLineF lineToTarget(QPointF(ZERO, ZERO), mapFromScene(target));
    // Угол поворота в направлении к цели
    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
    if (lineToTarget.dy() < ZERO)
        angleToTarget = TwoPi - angleToTarget;
    angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

    /* В Зависимости от того, слева или справа находится Цель от Героя,
     * устанавливаем направление поворота Героя в данном тике таймера
     * */
    if (angleToTarget >= ZERO && angleToTarget < Pi) {
        // Rotate left
        setRotation(rotation() - angleToTarget * 180 /Pi);
    } else if (angleToTarget <= TwoPi && angleToTarget > Pi) {
        // Rotate right
        setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
    }


}

Player::Player(std::vector <Wall> walls, double health, QObject *parent) : QObject(parent)
{
    this->walls = walls;
    angle = ZERO;     // Задаём угол поворота графического объекта
    setRotation(angle);     // Устанавилваем угол поворота графического объекта
    this->health = health;
    maxHealth =health;
    go = 'g';
    shot = false;
    //gameTimer = new QTimer();   // Инициализируем игровой таймер
    // Подключаем сигнал от таймера и слоту обработки игрового таймера
    //connect(gameTimer, &QTimer::timeout, this, &Player::slotGameTimer);
    //gameTimer->start(10);   // Стартуем таймер

    checkTimer = new QTimer();
    connect(checkTimer, &QTimer::timeout, this, &Player::check);
    checkTimer->start(10);

    bulletTimer = new QTimer(); // Инициализируем таймер создания пуль
    connect(bulletTimer, &QTimer::timeout, this, &Player::slotBulletTimer);
    bulletTimer->start(100); // Стреляем 6 раз в секунду


}
Player::~Player(){

}


void Player::check(){

    if(health < ZERO){
        health = ZERO;
        shot = false;
        emit endOfLife("player");
        this->deleteLater();
    }

    if(!scene()->collidingItems(this).isEmpty()){
        if(go=='l' ){

            setPos(mapToParent(10, ZERO));
            if(!scene()->collidingItems(this).isEmpty()){
                setPos(mapToParent(-15, ZERO));
            }

        }

        else if(go=='r'){

            setPos(mapToParent(-10, ZERO));
            if(!scene()->collidingItems(this).isEmpty()){
                setPos(mapToParent(15, ZERO));
            }

        }
        else if(go=='g' ){

            setPos(mapToParent(ZERO, 10));
            if(!scene()->collidingItems(this).isEmpty()){
                setPos(mapToParent(ZERO, -15));
            }

        }
        else if(go=='d'){

            setPos(mapToParent(ZERO, -10));
            if(!scene()->collidingItems(this).isEmpty()){
                setPos(mapToParent(ZERO, 15));
            }

        }
    }
}


void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    QPolygon polygon;
    polygon << QPoint(0,-15) << QPoint(10,15) << QPoint(-10,15);
    painter->setBrush(Qt::red);
    painter->drawPolygon(polygon);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::green);
    //painter->drawPoint(this->x(), this->y()+1);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::blue);
    painter->drawRect(-20, 20, (int) 40*health/maxHealth, 3);
    Q_UNUSED(option);
    Q_UNUSED(widget);

}
QPainterPath Player::shape() const
{

    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;

}

QRectF Player::boundingRect() const
{
    return QRectF(-20,-30,40,60); /// Ограничиваем область, в которой лежит треугольник
}


void Player::Go(char go){
    //check();
    if(go=='l'){

        setPos(mapToParent(-10, ZERO));


    }

    else if(go=='r'){

        setPos(mapToParent(10, ZERO));

    }
    else if(go=='g' && !checkWay(this->pos(), QPointF(this->pos().x(), this->pos().y() - 10), walls)){

        setPos(mapToParent(ZERO, -10));

    }
    else if(go=='d'){

        setPos(mapToParent(ZERO, 10));

    }
    this->go = go;
    check();
}



void Player::hit(int damage)
{
    health -= damage;   // Уменьшаем здоровье мишени
    this->update(QRectF(-20,20,40,40));    // Перерисовываем мишень
    // Если здоровье закончилось, то инициируем смерть мишени

}

void Player::slotBulletTimer()
{
    // Если стрельба разрешена, то вызываем сигнал на создание пули
    if(shot) emit signalBullet("player", mapToScene(ZERO,-40), target);

}

void Player::slotShot(bool shot)
{
    this->shot = shot;  // Получаем разрешение или запрет на стрельбу
}

Target Player::getValue(){
    Target one(this->pos(), "player");
    return one;
}
