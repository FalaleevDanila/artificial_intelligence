#pragma once

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>

/*!
    \brief Класс пуля, отвечает за создание пули, перемещение и взаимодействие её с другими объектами.

    Объект данного класс создается при вызове конструктора, а затем непрерывно перемещается по полю.
    Объект хранит информацию объекта его создавшего (Npc или Player), эту информацию он передает объекту
    с которым он столкнулся (при наличии слота передачи).

*/
class Bullet : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    /*!
    Создает пулю.
    \param attackerName - запоминает имя создателя пули, точки start и end задают траекторию полёта пули.
    */
    explicit Bullet(std::string attackerName, QPointF start, QPointF end, QObject *parent = nullptr);
    ~Bullet();
    /*!
    Передает имя объъекта его создавшего.
    \return имя обекта его создавшего.
    */
    std::string getAttackerName();

protected:
    /*!
    Функция для обозначчения объекта на поверхности.
    \return Область в которой лежит тело.
    */
    QRectF boundingRect() const;
    /*!
    Функция отрисовки пули.
    */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected slots:
    /*!
    Слот для обработки полёта пули.
    */
    void slotTimerBullet();

protected:

    QTimer *timerBullet;
    std::string attackerName; // имя создателя

};



