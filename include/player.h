#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QTimer>


#include "wall.h"
#include "man.h"


/*!
    \brief Класс игрока, объектом этого класса вы будете управлять

    Предосталяет пользователю участвовать в процессе тестирован ИИ Npc, перемещаясь по полю, становясь при желании целью для атаки.
*/
class Player : public QObject, public QGraphicsItem, public Man
{
    Q_OBJECT

public:
    /*!
    Создает объект типа Player.
    \param health - максимальное здоровье, walls - препятствия на игровой карте.
    */
    explicit Player(std::vector <Wall> walls, double health, QObject *parent = nullptr);
    ~Player();


    /*!
    Функция вызываемая, при попадании в этот объект.
    \param neme - имя выстрелившего в этот объект, damage - урон, нанесенный пулей.
    */
    void hit(int damage);


    /*!
    Функция получения информации об объекте.
    \return Имя объекта и его коодинаты.
    */
    Target getValue();

protected:

    /*!
    Отрисовка Playerc-объекта.
    */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

    /*!
    Сигнал отвечающий за создание пули.
    \param neme - имя объекта, точки start и end задают траектор полета пули.
    */
    void signalBullet(std::string getName, QPointF start, QPointF end);


    /*!
    Сигнал отвечающий за уведомленя игры о смерти объекта.
    */
    void endOfLife(std::string name);

public slots:

    /*!
    Слот перемещения игрка по полю.
    \param go - нажатая клавиша.
    */
    void Go(char go);


    /*!
    Слот проверки пули.
    */
    void slotTarget(QPointF point);


    /*!
    Слот проверки состояния стрельбы.
    */
    void slotShot(bool shot);

private slots:

    /*!
    Слот проверки разрешения стрельбы.
    */
    void slotBulletTimer(); // Слот проверки пули

private:
    /*!
    Преобразует прямоугольник из метода shape() в овал.
    \return Овал, в область которого на игровом поле помещен объект.
    */
    QRectF boundingRect() const;

    /*!
    Функция вычисляет диапазон, в котором лежит объект
    \return Прямоугольник, в область которого на игровом поле помещен объект.
    */
    QPainterPath shape() const;

    /*!
    Слот проверки на коллизии.
    */
    void check();

private: //

    char go;
    qreal angle;              // Угол поворота графического объекта
    QPointF target;           // Положение курсора

    bool shot;                // Переменная состояния стрельбы


    QTimer *bulletTimer;      // Таймер пули
    QTimer *gameTimer;        // Таймер главного процесса
    QTimer *checkTimer;       // Таймер проверки


    double health;            // здоровье
    double maxHealth;         // максимальное здоровье
    std::vector <Wall> walls; // объекты на игровой сцене


};

#endif // PLAYER_H
