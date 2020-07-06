#include "pinmonitor.h"
#include "ui_pinmonitor.h"
#include "mainwindow.h"
#include <QGraphicsItem>

#include <QPaintEvent>
PinMonitor::PinMonitor(Pin * pin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PinMonitor)
{
    ui->setupUi(this);
    this->pin = pin;
    ui->monitor->setPin(pin);
    ui->name->setText(pin->name);
    ui->addr->setText(QString("0x"+QString::number(pin->addr,16).toUpper()));
    ui->bit->setNum(pin->bit);
    ui->level->setText(pin->level()? "HIGH" : "LOW");

    setWindowTitle(QString("Монитор состояния пина %1").arg(pin->name));
}

PinMonitor::~PinMonitor()
{
    delete ui;
}

void PinMonitor::paintEvent(QPaintEvent *event)
{
    ui->level->setText(pin->level()? "HIGH" : "LOW");
    event->ignore();
}

void PinMonitor::on_timeScale_valueChanged(int arg1)
{
    ui->monitor->setTimeScale(arg1);
}














