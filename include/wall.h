#ifndef WALL_H
#define WALL_H

/*!
    \brief Класс стена, участвует в создании стен на игровом поле, а также в обнаружений коллизий у ИИ Npc.

    Класс стена, участвует в создании стен на игровом поле, а также в обнаружений коллизий у ИИ Npc.
*/
struct Wall
{
    double x;      // координата стартовой точки по x
    double y;      // координата стартовой точки по y
    double width;  // ширина
    double height; // длина

    Wall() = delete;

    /*!
    Создает объект типа Npc.
    \param (x, y) - координаты точки начала,  width - ширина стены, height - длина стены.
    */
    Wall(double x, double y, double width, double height): x(x),
                                                           y(y),
                                                           width(width),
                                                           height(height)
    {}
};

#endif // WALL_H
