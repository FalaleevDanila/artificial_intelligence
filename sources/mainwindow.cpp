#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap myPixmap( "main.png" );
    MainWindow::ui->label_2->setScaledContents(true);
    MainWindow::ui->label_2->setPixmap(myPixmap);

    //widget = new Widget();

    //connect(widget, &Widget::exit, this, &MainWindow::show);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_play_clicked()
{
    Widget *widget = new Widget();
    connect(widget, &Widget::exit, this, &MainWindow::show);
    connect(widget, &Widget::repit, this, &MainWindow::repit_game_slot);
    widget->show();
    this->close();
}

void MainWindow::on_exit_clicked()
{
    QMessageBox::StandardButton chec = QMessageBox::question(this,
                                                             " Notification ",
                                                             " Are you sure? ",
                                                             QMessageBox::Yes
                                                             | QMessageBox::No);

    if(chec==QMessageBox::Yes){
       this->close();
    }
}

void MainWindow::repit_game_slot(){
    //widget->deleteLater();
    on_play_clicked();
}
