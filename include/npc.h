#ifndef NPC_H
#define NPC_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>
#include <QTimer>

#include "man.h"
#include "way.h"
#include "wall.h"
/*!
    \brief Класс Npc с искусственным интеллектом

    Объекты данного класса управляются с помощью искусственного интеллекла,
    сформированного на основе предоставленных при создании данных.
*/
class Npc : public QObject, public QGraphicsItem, public Man
{
    Q_OBJECT
public:

    /*!
    Создает объект типа Npc.
    \param name - имя объекта, NpassiveWay - маршрут объекта, когда он находится в покое,
    \param agressive - агрессивность оъекта, step - размер шага, enemNames - первоначальные враги, walls - препятствия на игровой карте.
    */
    explicit Npc(QObject *parent, std::string name, Way NpassiveWay, bool agressive, int step, std::vector<std::string> enemNames, std::vector <Wall> walls);
    ~Npc();

    /*!
    Функция вызываемая, при попадании в этот объект.
    \param neme - имя выстрелившего в этот объект, damage - урон, нанесенный пулей.
    */
    void hit(int damage, std::string name);

    /*!
    Функция получения имени.
    \return Имя данного объекта.
    */
    std::string getName();


    /*!
    Функция запоминания нового врага.
    \param attakerName - имя нападавшего.
    */
    void setAttackerName(Target attakerName);


    /*!
    функция поиска цели, после того как она была потеряна.
    */
    void findTarget();


    /*!
    Функция получения информации об объекте.
    \return Имя объекта и его коодинаты.
    */
    Target getValue();

    /*!
    Функция анализа живых мишеней.
    \param targets - доступные мишени.
    */
    void attackAnalizeFunction(
            std::vector <Target> targets);

    /*!
    Вычисляет длину отрезка
    \param first и second - точки-концы отрезка, длину которого надо найти.
    \return Длину отрезка.
    */
    static double getLenght(QPointF first, QPointF second);

protected:

    /*!
    Отрисовка Npc-объекта.
    */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


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
    Функция, отвечающая за атаку на противников.
    */
    void agressiveFunction();


    /*!
    Функция, отвечающая за передвижение объекта в состоянии покоя по заданному маршрута.
    */
    void passiveFunction();


    /*!
    Функция, отвечающая за передвижение объекта в состоянии покоя до заданного маршрута (если он не доступен из-за коллизий).
    */
    void returnOnPassiveWay();

signals:

    /*!
    Сигнал отвечающий за создание пули.
    \param neme - имя объекта, точки start и end задают траектор полета пули.
    */
    void signalBullet(std::string name,
                      QPointF start,
                      QPointF end);


    /*!
    Сигнал отвечающий за уведомленя игры о смерти объекта.
    */
    void endOfLife(std::string name);

public slots:


    /*!
    Слот отвечающий за шаг влево.
    */
    void goLeft();


    /*!
    Слот отвечающий за шаг вправо.
    */
    void goRight();


    /*!
    Слот отвечающий за шаг вперед.
    */
    void goUp();


    /*!
    Слот отвечающий за шаг назад.
    */
    void goDown();



    /*!
    Слот отвечающий за шаг стрельбу.
    \param Переменная состояния стрельбы.
    */
    void slotShot(bool shot);


    /*!
    Слот обработки игры.
    */
    void slotGameTimer();



protected slots:


    /*!
    Слот проверки пули.
    */
    void slotBulletTimer();

    /*!
    Слот отслеживающий положение цели.
    */
    void focusRotationSlot();

protected:

    /*!
      Функция проверки на коллизии.
    */
    void check();


    /*!
    Функция перемещения.
    */
    void Go();


protected:

    int step;                               // Длина шага объекта
    int sv;                                 // Диапазон забывчивости запоминания маршрута
    std::string name;                       // Имя объекта
    qreal angle;                            // Угол поворота графического объекта


    Target target;                          // Положение курсора
    QPointF bulletTarget;                   //
    QPointF playerCoordinate;               // координата в которой был замечен игрок


    bool shot;                              // Переменная состояния стрельбы
    bool agressive;                         // Переменная состояния дружелюбия моба


    bool nowAgressive;                      // Переменная состояния агрессии моба в настоящий момент
    bool onWay;                             // Переменная состояния поиска
    bool haveTarget;                        // Переменная состояния цели
    bool lastTargetPoint;                   // Переменная состояния достижения последней точки, где была замечена цель


    /// таймеры
    QTimer *bulletTimer;
    QTimer *gameTimer;
    QTimer *checkPlayer;
    QTimer *checkTimer;
    QTimer *focusRotationTimer;

    char go;

    qreal checkAngle;                       // переменная поиска цели на последней её позиции
    Way passiveWay;                         // вектор перемещения npc в спокойном состояни
    Way agressiveWay;                       // вектор перемещения npc во время погони за игроком

    double pastTargetLenght;
    double health;                          // здоровье
    double maxHealth;                       // максимальное здоровье
    int tm;

    std::vector <Target> enemies;           // вектор с информацией о других участниках сцены
    std::vector <std::string> enemiesNames; // вектор имен врагов
    std::vector <Wall> walls;               // вектор препятсятвий на игровой карте

};

#endif // NPC_H
