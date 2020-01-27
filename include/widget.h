#pragma once

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsView>

#include <QDebug>

#include "mainwindow.h"
#include "npc.h"
#include "player.h"
#include "mousecontrol.h"
#include "bullet.h"
#include "npc_way.h"
#include "man.h"


class MainWindow;
class Player;
class Npc;

namespace Ui {
class Widget;
}


/*!
    \brief Класс игрового поля, в нем будет происходить сама игра

    Данный класс отвечает за создание всех объектов (таких как Player, Npc и Walls),
    за их взаимодействие, является посредником передачи информации между объектами типа Man
*/
class Widget : public QWidget
{
    Q_OBJECT

public:
    /*!
    Создает объект типа Widget,  вызывает init().
    */
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;


signals:
    /*!
    Сигнал закрытия окна.
    */
    void exit();

    /*!
    Сигнал нажатой клавиши.
    \param go - нажатая клавиша.
    */
    void playerGo(char go);

    /*!
    Сигнал перезапуска ишры.
    */
    void repit();

private slots:

    /*!
    Слот отвечающий за создание пули.
    \param neme - имя объекта, точки start и end задают траектор полета пули.
    */
    void slotBullet(std::string attackerName, QPointF start, QPointF end);

    /*!
    Слот передающий информацию каждому npc о положении других объектов.
    */
    void sceneBatleAnalizer();

    /*!
    Сигнал отвечающий за признание объектов "не рентабельными)".
    \param neme - имя убитого объекта.
    */
    void endOfLifeSlot(std::string name);

    /*!
    Слот окончания игры.
    */
    void endSlot();

public slots:
    /*!
    Обрабатывает нажатие клавиш.
    \param event - нажатая клавиша.
    */
    void keyPressEvent(QKeyEvent *event) override;

private:
    /*!
    Функция инициализации игровой сцены.
    */
    void init();

    
private:

    Ui::Widget *ui;
    MouseControl *scene;     // Объявляем графическую сцену
    Player *player;          // игрок

    /// таймеры
    QTimer *battleAnalizerTimer;
    QTimer *endTimer;

    std::vector <Man*> men; // вектор из объектов

    
    Npc *npc1;              // просто гуляющий объект
    Npc *npc2;              // агрессивный сторож
    Npc *npc3;              // агрессивный сторож, который движется по определенной траектории

    bool end;


};


