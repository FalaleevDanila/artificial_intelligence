#ifndef COLLISING_H
#define COLLISING_H

#include <QPointF>
#include <vector>

#include "wall.h"

static double area (QPointF a, QPointF b, QPointF c) {

    return (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
}

static bool intersect_1 (double a, double b, double c, double d) {

    if (a > b)  std::swap (a, b);
    if (c > d)  std::swap (c, d);
    return std::max(a,c) <= std::min(b,d);
}

static bool intersect (QPointF a, QPointF b, QPointF c, QPointF d) {

    return intersect_1 (a.x(), b.x(), c.x(), d.x())
        && intersect_1 (a.y(), b.y(), c.y(), d.y())
        && area(a,b,c) * area(a,b,d) <= 0
        && area(c,d,a) * area(c,d,b) <= 0;
}


/*!
Проверяет на пересечение отрезка, заданного точками, других объектов.
\param first и second - точки-концы отрезка, который проверяют на рпересечения с walls.
\return true если есть пересечение, false в противном случае.
*/
static bool checkWay(QPointF first, QPointF second, std::vector<Wall> walls) {

    bool pw = true;

    for (auto it = walls.begin(); it != walls.end(); ++it) {


        pw = intersect(first, second,
                          QPointF(it->x, it->y), QPointF((it->x + it->width), it->y));
        if(!pw == false) break;

        pw = intersect(first, second,
                          QPointF(it->x, it->y), QPointF(it->x, it->y + it->height));
        if(!pw == false) break;


        pw = intersect(first, second,
                          QPointF(it->x  + it->width, it->y + it->height), QPointF(it->x + it->width, it->y));
        if(!pw == false) break;


        pw = intersect(first, second,
                          QPointF(it->x  + it->width, it->y + it->height), QPointF(it->x, it->y + it->height));
        if(!pw == false) break;

        pw = intersect(first, second,
                          QPointF(it->x, it->y), QPointF( it->x + it->width, it->y + it->height));
        if(!pw == false) break;

        pw = intersect(first, second,
                          QPointF(it->x  + it->width, it->y), QPointF(it->x, it->y + it->height));
        if(!pw == false) break;

    }
    return pw;

}

#endif // COLLISING_H
