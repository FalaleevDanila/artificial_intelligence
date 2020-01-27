#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>

#include "widget.h"


class Widget;

namespace Ui {
    class MainWindow;
}


/*!
    \brief Данный класс отвечает за создание игрового поля, выхода и повтора игры.

    Данный класс отвечает за создание игрового поля, выхода и повтора игры.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    /*!
    Сигнал начала игры.
    */
    void play();

private slots:
    /*!
    Слот отвечающий за начало игры.
    */
    void on_play_clicked();

    /*!
    Слот отвечающий за повтор игры.
    */
    void repit_game_slot();

    /*!
    Слот отвечающий за окончание работы приложения.
    */
    void on_exit_clicked();

private:
    Ui::MainWindow *ui;

    Widget *widget;

};


