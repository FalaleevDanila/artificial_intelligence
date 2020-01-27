#ifndef MAN_H
#define MAN_H

#include "target.h"
/*!
    \brief Родительский класс, от него унаследованы Player и Npc.

    Родительский класс, от него унаследованы Player и Npc.
*/
class Man
{
public:
    /*!
    Функция получения информации об объекте.
    \return Имя объекта и его коодинаты.
    */
    virtual Target getValue(){
        return Target();
    }
    virtual ~Man(){}

};

#endif // MAN_H
