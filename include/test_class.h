#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include <QObject>


class Test_class: public QObject
{
    Q_OBJECT

public:

    explicit Test_class(QObject *parent = nullptr);

private slots:

    void test1();

    void test2();

    void test3();
};

#endif // TEST_CLASS_H
