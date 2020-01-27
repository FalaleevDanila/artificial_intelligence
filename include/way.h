#ifndef WAY_H
#define WAY_H

#include <QPointF>
#include <vector>
#include <algorithm>
#include <iostream>

/*!
    \brief Структура пути, отвечающая за цель у ИИ Npc

    Осуществляет функционал перемещения по циклу, от первой точки к последней
    и наоборот или простого строяния.
*/

struct Way
{

    bool inf;                    // параметр состояния маршрута - бесконечность (доходит до последней точки и идёт назад
    bool cycle;                  // параметр состояния маршрута - цикл
    std::vector <QPointF> way;   // путь по которому идёт передвижение
    std::vector <QPointF> copy;  // копия пути для безопасной работы

public:

    Way(){}


    /*!
    Создает объект типа Way.
    \param inf - параметр состояния маршрута - бесконечность, cycle - параметр состояния маршрута - цикл, way - маршрут по которому идёт передвижение.
    */
    Way(bool inf, bool cycle, std::vector <QPointF> way){

        this->inf   =  inf;
        this->cycle =  cycle;
        this->way   =  way;
        this->copy =   way;
    }


    /*!
    Преходит к следующей точке маршрута.
    */
    void next(){

        if(copy.size() != 0){

            if(copy.size() == 1){

                if(inf == true){

                    wayReverse();
                    auto iter2=copy.begin();
                    copy.erase(iter2);
                }
                else if(cycle == true){

                    copy = way;
                }
                else {

                    auto iter = copy.begin();
                    copy.erase(iter);
                }
            }
            else {

                auto iter = copy.begin();
                copy.erase(iter);
            }
        }
    }


    /*!
    Перезаписывает координаты точек маршрута в обратном направлении.
    */
    void wayReverse(){
        std::reverse(way.begin(),way.end());
        copy = way;
    }
    ~Way(){}
};

#endif // WAY_H
