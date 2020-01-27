#ifndef TARGET_H
#define TARGET_H

#include <QPointF>
#include <string>

/*!
    \brief Класс Цели, хранит координаты цели и имя объекта, связанного с этой целью.

    Класс Цели, хранит координаты цели и имя объекта, связанного с этой целью.
*/
struct Target
{

    std::string name; // наименование цели
    QPointF position; // координаты цели


public:
    Target() = default;
    /*!
    Создает объект типа Target.
    \param position - координаты цели, name - имя цели.
    */
    Target(QPointF position, std::string name):
        name(name),
        position(position)

    {}

};

#endif // TARGET_H
