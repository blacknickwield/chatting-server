#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <server.h>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_btn_start_clicked()
{

    Server* server = Server::getInstance();
    server->start(6666, 8200);
    ui->btn_start->setEnabled(false);
    ui->btn_stop->setEnabled(true);
    qDebug() << "start";
}

void MainWidget::on_btn_stop_clicked()
{
    Server* server = Server::getInstance();
    server->stop();
    ui->btn_stop->setEnabled(false);
    ui->btn_start->setEnabled(true);
    qDebug() << "stop";
}
