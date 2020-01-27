#include "include/widget.h"
#include "include/mainwindow.h"
#include <QApplication>
#include "test_class.h"

#include <QTest>

int main(int argc, char *argv[])
{

    //======
    freopen("testing.log", "w", stdout);
    QApplication a(argc, argv);
    QTest::qExec(new Test_class, argc, argv);
    //======
    //QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
