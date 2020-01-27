#include "test_class.h"

#include <QTest>
#include "wall.h"
#include "npc.cpp"
#include "collising.h"

Test_class::Test_class(QObject *parent): QObject(parent) {}

void Test_class::test1() {
    //QCOMPARE(a.max(1,   0), 1);
    std::vector<Wall> walls;

    walls.push_back(Wall(  -10, -10,   10, 10));


    QCOMPARE(checkWay(QPointF(-20, 0), QPointF(20, 0), walls), true);
    QCOMPARE(checkWay(QPointF(  0, 20), QPointF(0, -20), walls), true);
    QCOMPARE(checkWay(QPointF(  20, 20), QPointF(40, 40), walls), false);


}

void Test_class::test2() {

    std::vector <QPointF> passiveNpc1Way;

    passiveNpc1Way.push_back(QPointF(  500,    0));
    passiveNpc1Way.push_back(QPointF(  500, -200));
    passiveNpc1Way.push_back(QPointF(    0, -200));
    passiveNpc1Way.push_back(QPointF(    0,    0));


    QCOMPARE(passiveNpc1Way[0], QPointF(500,    0));
    QCOMPARE(passiveNpc1Way[1], QPointF(500, -200));
    QCOMPARE(passiveNpc1Way[2], QPointF(  0, -200));
    QCOMPARE(passiveNpc1Way[3], QPointF(  0,    0));

    Way way1(false, true, passiveNpc1Way);

    QCOMPARE(way1.copy[0], QPointF(  500,    0));
    way1.next();
    QCOMPARE(way1.copy[0], QPointF(  500, -200));
    way1.next();
    QCOMPARE(way1.copy[0], QPointF(    0, -200));
    way1.next();
    QCOMPARE(way1.copy[0], QPointF(    0,    0));
    way1.next();
    QCOMPARE(way1.copy[0], QPointF(  500,    0));
    way1.next();
    QCOMPARE(way1.copy[0], QPointF(  500, -200));



    Way way2(true, false, passiveNpc1Way);

    QCOMPARE(way2.copy[0], QPointF(  500,    0));
    way2.next();
    QCOMPARE(way2.copy[0], QPointF(  500, -200));
    way2.next();
    QCOMPARE(way2.copy[0], QPointF(    0, -200));
    way2.next();
    QCOMPARE(way2.copy[0], QPointF(    0,    0));
    way2.next();
    QCOMPARE(way2.copy[0], QPointF(    0, -200));
    way2.next();
    QCOMPARE(way2.copy[0], QPointF(  500, -200));
}

void Test_class::test3() {

    QCOMPARE(normalizeAngle(TwoPi+1), 1);
    QCOMPARE(normalizeAngle(TwoPi+2), 2);

    QCOMPARE(normalizeAngle(TwoPi+1.5), 1.5);
    QCOMPARE(normalizeAngle(TwoPi+0.5), 0.5);


    QCOMPARE(normalizeAngle(TwoPi), TwoPi);
    QCOMPARE(normalizeAngle(-TwoPi), 0);

    QCOMPARE(normalizeAngle(-TwoPi/2), TwoPi/2);
}
