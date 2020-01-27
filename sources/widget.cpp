#include "include/widget.h"
#include "ui_widget.h"

#include <cmath>
#include <QDebug>
#include <iostream>

#include "way.h"
#include "wall.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    init();
    end = false;

}

void Widget::init(){
    ui->setupUi(this);
    this->resize(1900,1000);          /// Задаем размеры виджета, то есть окна
    scene = new MouseControl();   /// Инициализируем графическую сцен


    ui->graphicsView->setScene(scene);  /// Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    /// Устанавливаем сглаживание

    scene->setSceneRect(0,0,0,0); /// Устанавливаем область графической сцены
    std::vector<Wall> walls;

    walls.push_back(Wall(  920, -450,   30, 900));
    walls.push_back(Wall( -950, -450,   30, 900));
    walls.push_back(Wall( -950,  450, 1900,  30));
    walls.push_back(Wall( -920, -450, 1870,  30));
    walls.push_back(Wall(   80, -450,   30, 150));
    walls.push_back(Wall(   140, 100,   30,  250));
    walls.push_back(Wall(   140, 350,   150,  30));
    walls.push_back(Wall( -800, -450,   30, 150));
    walls.push_back(Wall( -650, -150,   30, 450));
    walls.push_back(Wall( -350, -300,   30, 250));
    walls.push_back(Wall(  -350,  100,  1100,  30));
    walls.push_back(Wall(  750,  -450,  30, 580));

    //walls.push_back(Wall(  850,  -450,  30, 580));


    for(auto it = walls.begin(); it != walls.end(); ++it) {

        scene->addRect( it->x, it->y, it->width, it->height, QPen(Qt::NoPen),QBrush(Qt::darkBlue));
    }

    Player *player = new Player(walls, 100);
    scene->addItem(player);        /// Добавляем на сцену игрока
    player->setPos(-100,30);      /// Устанавливаем игрока в соответствующие координаты

    connect(this, &Widget::playerGo, player , &Player::Go);



    std::vector <QPointF> passiveNpc1Way;

    passiveNpc1Way.push_back(QPointF(500,0));
    passiveNpc1Way.push_back(QPointF(500,-200));
    passiveNpc1Way.push_back(QPointF(0,-200));
    passiveNpc1Way.push_back(QPointF(0,0));

    Way way1(false, true, passiveNpc1Way);
    std::vector <std::string> npc1_start_enemies;


    npc1 = new Npc(nullptr, "npc1", way1, false, 2, npc1_start_enemies, walls);

    std::vector <QPointF> passiveNpc2Way;

    passiveNpc2Way.push_back(QPointF(850,  -350));

    Way way2(false, false, passiveNpc2Way);
    std::vector <std::string> npc2_start_enemies;

    npc2 = new Npc(nullptr, "npc2", way2, false, 2, npc2_start_enemies, walls);


    std::vector <QPointF> passiveNpc3Way;

    passiveNpc3Way.push_back(QPointF(150,  400));
    passiveNpc3Way.push_back(QPointF(850,  400));
    passiveNpc3Way.push_back(QPointF(850,  -350));

    Way way3(true, false, passiveNpc3Way);
    std::vector <std::string> npc3_start_enemies{"npc2"};

    npc3 = new Npc(nullptr, "npc3", way3, false, 2, npc3_start_enemies, walls);

    scene->addItem(npc1);                          // Добавляем на сцену нпс
    scene->addItem(npc2);                          // Добавляем на сцену нпс
    scene->addItem(npc3);                          // Добавляем на сцену нпс

    npc1->setPos(0, 0);                            // Устанавливаем нпс в соответствующие координаты
    npc2->setPos(850, -350);                            // Устанавливаем нпс в соответствующие координаты
    npc3->setPos(150, 400);                            // Устанавливаем нпс в соответствующие координаты

    ui->graphicsView->setMouseTracking(true);

     // Соединяем сигнала стрельбы с графической сцены со слотом разрешения стрельбы игрока
    connect(scene, &MouseControl::signalShot, player, &Player::slotShot);
     // Соединяем сигнал на создание пули со слотом, создающим пули в игре
    connect(player, &Player::signalBullet, this, &Widget::slotBullet);

    // Соединяем сигнал на создание пули со слотом, создающим пули в игре
    connect(npc1, &Npc::signalBullet, this, &Widget::slotBullet);
    connect(npc2, &Npc::signalBullet, this, &Widget::slotBullet);
    connect(npc3, &Npc::signalBullet, this, &Widget::slotBullet);

    connect(scene, &MouseControl::signalTargetCoordinate, player, &Player::slotTarget);

    battleAnalizerTimer = new QTimer();
    connect(battleAnalizerTimer, &QTimer::timeout, this, &Widget::sceneBatleAnalizer);

    battleAnalizerTimer->start(10);

    endTimer = new QTimer();
    connect(endTimer, &QTimer::timeout, this, &Widget::endSlot);
    endTimer->start(10);

    connect(npc1, &Npc::endOfLife, this, &Widget::endOfLifeSlot);
    connect(npc2, &Npc::endOfLife, this, &Widget::endOfLifeSlot);
    connect(npc3, &Npc::endOfLife, this, &Widget::endOfLifeSlot);

    connect(player, &Player::endOfLife, this, &Widget::endOfLifeSlot);

    men.push_back(player);
    men.push_back(npc1);
    men.push_back(npc2);
    men.push_back(npc3);
}



Widget::~Widget()
{
    delete ui;
}

void Widget::endOfLifeSlot(std::string name) {
    for(auto iter1 = men.begin(); iter1!=men.end(); ++iter1) {

        if((*iter1)->getValue().name == name){

            men.erase(iter1);
            break;
        }
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{

   int key = event->key();
   if (key == Qt::Key_Tab) {
       emit exit();
       this->close();
       //this->deleteLater();
   }
   if(key == Qt::Key_A){

       emit playerGo('l');
                                       /* Продвигаем объект на 15 пискселей влево
                                        * перетранслировав их в координатную систему
                                        * графической сцены
                                        * */
   }
   if(key == Qt::Key_D){

       emit playerGo('r');
                                       /* Продвигаем объект на 15 пискселей вправо
                                        * перетранслировав их в координатную систему
                                        * графической сцены
                                        * */
   }
   if(key == Qt::Key_W){

       emit playerGo('g');
                                       /* Продвигаем объект на 15 пискселей вперёд
                                        * перетранслировав их в координатную систему
                                        * графической сцены
                                        * */
   }
   if(key == Qt::Key_S){

       emit playerGo('d');
                                       /* Продвигаем объект на 15 пискселей назад
                                        * перетранслировав их в координатную систему
                                        * графической сцены
                                        * */
   }
}

void Widget::slotBullet(std::string attackerName, QPointF start, QPointF end) {
    // Добавляем на сцену пулю

    scene->addItem(new Bullet(attackerName ,start, end));
}



void Widget::sceneBatleAnalizer() {


    if(men.size() > 0) {
        for(auto iter1 = men.begin(); iter1!=men.end();++iter1) {

            if(*iter1 != nullptr){

                if(men.size() > 1) {
                    Target first = (*iter1)->getValue();
                    std::vector <Target> enemies;     // вектор из врагов

                    if(first.name != "player"){
                        for(auto iter2 = men.begin(); iter2!=men.end();++iter2) {

                            if(*iter2 != nullptr){
                                Target second = (*iter2)->getValue();

                                if(first.name != second.name){
                                    enemies.push_back(second);
                                }
                            }

                        }

                        if(enemies.size() > 0){
                            Npc *one = static_cast<Npc*>(*iter1);
                            one->attackAnalizeFunction(enemies);
                        }

                        enemies.clear();
                    }
                }
            }
        }
    }
}


void Widget::endSlot(){
    if(end == false){
        auto haveplayer = std::find_if(men.begin(), men.end(), [](Man* man) {
            return (*man).getValue().name == "player";
        });

        if(men.size()==1 || haveplayer == men.end()){
            end = true;

            auto iter1 = men.begin();
            if((*iter1)->getValue().name == "player"){
                QMessageBox::StandardButton chec = QMessageBox::question(this,
                                                                         " You Win!!! ",
                                                                         " Repit ?;) ",
                                                                         QMessageBox::Yes
                                                                         | QMessageBox::No);

                if(chec==QMessageBox::Yes){

                    Player *one = static_cast<Player*>(*iter1);
                    one->deleteLater();
                    men.clear();

                    emit repit();
                    this->close();
                    this->deleteLater();

                }
                else {
                    Player *one = static_cast<Player*>(*iter1);
                    one->deleteLater();
                    men.clear();

                    emit exit();
                    this->close();
                    this->deleteLater();
                }
            }
            else {

                QMessageBox::StandardButton chec = QMessageBox::question(this,
                                                                         " You Lose!!! ",
                                                                         " REPIT ? ",
                                                                         QMessageBox::Yes
                                                                         | QMessageBox::No);

                if(chec==QMessageBox::Yes){
                    Npc *one = static_cast<Npc*>(*iter1);
                    one->deleteLater();
                    men.clear();

                    emit repit();
                    this->close();
                    this->deleteLater();

                }
                else{
                    Npc *one = static_cast<Npc*>(*iter1);
                    one->deleteLater();
                    men.clear();

                    emit exit();
                    this->close();
                    this->deleteLater();
                }
            }
        }
    }
}


