#include "include/npc.h"
#include "iostream"
#include <math.h>
#include <QVector2D>
#include <QDebug>
#include "include/player.h"
#include "include/bullet.h"
#include "collising.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
static const double MAX_TARGET_LENGHT = 10000000000;
static const double ZERO = 0;

static qreal normalizeAngle(qreal angle)
{
    while (angle < ZERO)
           angle += TwoPi;

    while (angle > TwoPi)
           angle -= TwoPi;

    return angle;
}


Npc::Npc(QObject *parent, std::string name, Way NpassiveWay, bool agressive, int step, std::vector<std::string> enemNames, std::vector <Wall> walls) : QObject(parent)
{
    this->walls            = walls;
    this->enemiesNames     = enemNames;
    this->pastTargetLenght = MAX_TARGET_LENGHT;
    this->step             = step;
    this->name             = name;
    this->onWay            = true;
    this->haveTarget       = false;
    this->lastTargetPoint  = false;
    this->passiveWay       = NpassiveWay;
    tm                     = 0;
    angle                  = ZERO;                        //  Задаём угол поворота графического объекта
    setRotation(angle);                                   //  Устанавилваем угол поворота графического объекта
    this->agressive        = agressive;
    nowAgressive           = false;
    health                 = 100;
    maxHealth              = 100;
    shot                   = false;
    go                     = 'g';
    sv                     = 0;
    Target targetCopy(passiveWay.way[0],"");
    target                 = targetCopy;

    gameTimer = new QTimer();                             // Инициализируем игровой таймер
                                                          // Подключаем сигнал от таймера и слоту обработки игрового таймера
    connect(gameTimer, &QTimer::timeout, this, &Npc::slotGameTimer); // крашится тут
    gameTimer->start(10);                                 // Стартуем таймер

    bulletTimer = new QTimer();                           // Инициализируем таймер создания пуль
    connect(bulletTimer, &QTimer::timeout, this, &Npc::slotBulletTimer);
    bulletTimer->start(100);                             // Стреляем 10 раз в секунду

    checkPlayer = new QTimer();                           // Инициализируем таймер слежки
    connect(checkPlayer, &QTimer::timeout, this, &Npc::Go);
    checkPlayer->start(10);


    checkTimer = new QTimer();
    connect(gameTimer, &QTimer::timeout, this, &Npc::check);
    checkTimer->start(10);

    focusRotationTimer = new QTimer();
    connect(focusRotationTimer, &QTimer::timeout, this, &Npc::focusRotationSlot);
    focusRotationTimer->start(10);

}


void Npc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    /* Отрисовка треугольника
     * */
    QPolygon polygon;
    polygon << QPoint(0,-15) << QPoint(10,15) << QPoint(-10,15);

    painter->setBrush    (Qt::green);
    painter->drawPolygon (polygon);

    painter->setPen      (Qt::NoPen);
    painter->setBrush    (Qt::red);
    painter->drawRect    (-20,20, (int) 40*health/maxHealth,3);

    Q_UNUSED(option);
    Q_UNUSED(widget);

}


Npc::~Npc(){

}



void Npc::goLeft(){

    setPos(mapToParent(-1*step, ZERO));
                                            /* Продвигаем объект на 15 пискселей влево
                                             * перетранслировав их в координатную систему
                                             * графической сцены
                                             * */
    if(!scene()->collidingItems(this).isEmpty()) {

        this->setX(this->x() + 1*step);
    }

}


void Npc::goRight(){

    setPos(mapToParent(1*step, ZERO));
                                            /* Продвигаем объект на 15 пискселей вправо
                                             * перетранслировав их в координатную систему
                                             * графической сцены
                                             * */
    if(!scene()->collidingItems(this).isEmpty()) {

        this->setX(this->x() - 1*step);
    }
}


void Npc::goUp() {

    setPos(mapToParent(ZERO, -1*step));
                                            /* Продвигаем объект на 15 пискселей вперёд
                                             * перетранслировав их в координатную систему
                                             * графической сцены
                                             * */
    if(!scene()->collidingItems(this).isEmpty()) {

        this->setY(this->y() + 1*step);
    }
}


void Npc::goDown() {

    setPos(mapToParent(ZERO, 1*step));
                                            /* Продвигаем объект на 15 пискселей назад
                                             * перетранслировав их в координатную систему
                                             * графической сцены
                                             * */
    if(!scene()->collidingItems(this).isEmpty()) {

        this->setY(this->y() - 1*step);
    }
}


QPainterPath Npc::shape() const {

    QPainterPath path;
    path.addEllipse(boundingRect());

    return path;

}


QRectF Npc::boundingRect() const {

    return QRectF(-20,-30,40,60);   /// Ограничиваем область, в которой лежит объект
}


void Npc::slotGameTimer() {

    if(nowAgressive == false) {

        if(onWay) {

            passiveFunction();
        }
        else {

            returnOnPassiveWay();
        }
    }
    else if(nowAgressive == true) {


        agressiveFunction();
    }
}

qreal angleToTarget(QLineF lineToTarget) {

    qreal  angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());

    if (lineToTarget.dy() < ZERO)

           angleToTarget = TwoPi - angleToTarget;

           angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

    return angleToTarget;
}

void Npc::focusRotationSlot() {

    QLineF lineToTarget(QPointF(ZERO, ZERO), mapFromScene(target.position));

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

double Npc::getLenght(QPointF first, QPointF second) {
    return (sqrt((first.x()-second.x())
               * (first.x()-second.x())
               + (first.y()-second.y())
               * (first.y()-second.y())));
}

// функция атаки моба
void Npc::agressiveFunction() {
    sv = 0;
    shot = true;
    onWay = false;

    if(tm == 0 && go != 's') {
        agressiveWay.way.insert(agressiveWay.way.begin(), this->pos());
        tm ++;
    }
    else if(tm == 10) {
        tm = 0;
    }
    else {
        tm ++;
    }

    if(haveTarget) {

        if(getLenght(this->pos(), target.position) > 250.) {

            go = 'g';
            pastTargetLenght = MAX_TARGET_LENGHT;
        }
        else {
            go = 's';
        }
    }
    else {
        if(lastTargetPoint) {
            shot = false;
            findTarget();
        }
        else {

            agressiveWay.copy = agressiveWay.way;
            nowAgressive     = false;
            target.name      = "";
        }
    }
}


// функция для возврата в спокойное состояние
void Npc::passiveFunction(){

    if(passiveWay.way.size()==1 && getLenght(this->pos(), passiveWay.copy[0]) < step * 2){

        go = 's';
    }
    else{
        target.position = passiveWay.copy[0];

        go = 'g';
        double targetLenght  = getLenght(this->pos(), target.position);

        if(targetLenght > pastTargetLenght){

            passiveWay.next();
            target.position  = passiveWay.copy[0];
            pastTargetLenght = MAX_TARGET_LENGHT;
        }
        else {
            pastTargetLenght = targetLenght;
        }
    }
}


void Npc::returnOnPassiveWay(){
    shot = false;


    bool pw = checkWay(this->pos(), passiveWay.copy[0], walls);



    if (pw || sv < 7) {
        if(!pw) sv++;
        target.position       = agressiveWay.copy[0];
        go                    = 'g';
        double targetLenght   = getLenght(this->pos(), agressiveWay.copy[0]);

        if(targetLenght > pastTargetLenght){

            agressiveWay.next();
            pastTargetLenght  = MAX_TARGET_LENGHT;
        }
        else pastTargetLenght = targetLenght;

        onWay = false;
    }
    else {
        sv = 0;
        if(!checkWay(QPointF(this->pos().x()+3, this->pos().y()+3), passiveWay.copy[0], walls) &&
                checkWay(QPointF(this->pos().x()-3, this->pos().y()-3), passiveWay.copy[0], walls)){
            goLeft();
            goLeft();
            goLeft();
            goLeft();
        }
        else if(checkWay(QPointF(this->pos().x()+1, this->pos().y()+3), passiveWay.copy[0], walls) &&
                !checkWay(QPointF(this->pos().x()-1, this->pos().y()-3), passiveWay.copy[0], walls)){
            goRight();
            goRight();
            goRight();
            goRight();
        }
        agressiveWay.way.clear();
        target.name           = "";
        target.position       = passiveWay.copy[0];
        onWay                 = true;
        pastTargetLenght      = MAX_TARGET_LENGHT;
    }
}

void Npc::slotBulletTimer() {

    if (shot) {

        emit signalBullet(this->name, mapToScene(ZERO, -40), target.position);
        shot = false;
    }
}

void Npc::slotShot(bool shot){
    this->shot = shot;  // Получаем разрешение или запрет на стрельбу
}


void Npc::findTarget(){
    go = 'g';
    shot = false;
    if(getLenght(this->pos(), target.position) <= step*step || checkWay(this->pos(), target.position, walls)){
       lastTargetPoint = false;
    }
}

void Npc::hit(int damage, std::string name) {

    health -= damage;   // Уменьшаем здоровье мишени

    if(health > ZERO) {
        nowAgressive = true;
        bool fnd     = false;


        for(auto it = enemiesNames.begin(); it!= enemiesNames.end(); ++it){

            if(*it == name){
                fnd = true;
                break;

            }
        }
        if(!fnd){

            enemiesNames.push_back(name);
        }



        this->update(QRectF(-20,-20,40,40));    // Перерисовываем мишень
        // Если здоровье закончилось, то инициируем смерть мишени
    }

}

void Npc::check(){
    if(health < ZERO){
        health = ZERO;
        shot = false;
        emit endOfLife(this->name);
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

std::string Npc::getName(){
    return name;
}

void Npc::Go(){

    if (go == 'l'){

        goLeft();
    }

    else if (go == 'r'){

        goRight();
    }
    else if (go == 'g'){

        goUp();
    }
    else if (go == 'd'){

        goDown();
    }

}

Target Npc::getValue(){

    Target one(this->pos(), this->name);
    return one;
}



void Npc::attackAnalizeFunction(std::vector <Target> targets){

    bool agr = false;
    if(targets.size()!=0 && enemiesNames.size() != 0) {

        bool find = false;

        for (auto it = targets.begin(); it!=targets.end(); ++it) {
            if(it->name == target.name) {
                QLineF line(this->pos(), it->position);

                if((line.length() < 350 || (angleToTarget(line) < (Pi*3.5)/2 && line.length() < 700)) && !checkWay(this->pos(), it->position, walls)) {

                    target.position = it->position;
                    haveTarget      = true;
                    find  = true;
                    nowAgressive = true;
                    //shot  = true;
                    agr   = true;
                    onWay = false;
                    // agressiveWay.way.push_back(this->pos());
                    break;
                }
            }
        }
        haveTarget = find;
        if(agressive == false && !find) {

            shot = false;
            for (auto i = enemiesNames.begin(); i != enemiesNames.end(); ++i) {
                for (auto j = targets.begin(); j != targets.end(); ++j) {
                    if(*i == j->name){


                        QLineF line(this->pos(), j->position);

                        if((line.length() < 350 || (angleToTarget(line) < (Pi*3.5)/2 && line.length() < 700))&& !checkWay(this->pos(), j->position, walls)) {
                            target.position = j->position;
                            target.name     = j->name;
                            lastTargetPoint = true;
                            onWay           = false;
                            nowAgressive    = true;
                            agr             = true;
                            find            = true;

                            break;
                        }
                    }
                }
                if(agr == true){
                    break;
                }
            }
            haveTarget = find;
        }
        else if(!find) {

            shot = false;
            for (auto j = targets.begin(); j!=targets.end(); ++j) {

                QLineF line(this->pos(), j->position);
                if((line.length() < 350 || (angleToTarget(line) < (Pi*3.5)/2 && line.length() < 700)) && !checkWay(this->pos(), j->position, walls)){
                    target.position = j->position;
                    target.name     = j->name;
                    lastTargetPoint = true;
                    onWay = false;
                    agr = true;
                    nowAgressive = true;
                    //shot  = true;
                    find  = true;
                    // agressiveWay.way.push_back(this->pos());
                    break;
                }
            }
            haveTarget = find;

        }
    }
    //nowAgressive = agr;
}



void Npc::setAttackerName(Target attakerName) {
    enemies.push_back(attakerName);
}
