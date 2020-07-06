#include "monitorwidget.h"
#include "ui_monitorwidget.h"
#include "mainwindow.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPointF>

qreal HIGH = 30;
qreal LOW = 70;

MonitorWidget::MonitorWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MonitorWidget)
{
    ui->setupUi(this);
    startTimer(10);
}

MonitorWidget::~MonitorWidget()
{
    delete ui;
}

void MonitorWidget::setPin(Pin *pin)
{
    this->pin = pin;
}

void MonitorWidget::setTimeScale(quint8 sec){
    this->sec = sec;
    horizontal = sec <= 5 ? sec * 2 : sec % 10 == 0 ? 10 : sec;
    repaint();
}


void MonitorWidget::paintEvent(QPaintEvent * e)
{
    if(!pin)
        return;
    QPainter p(this);
    qreal offset = (w/(sec * 1000)) * 10;

    for(int i = 0 ; i < positions.size(); i ++)
    {
        // сдвигаем точки вправо
        positions[i].setX(positions.at(i).x()+offset);
        if(positions[i].rx() > w)
            positions.removeOne(positions[i]);
    }

    // нарисовать сетку
    p.setPen(Qt::gray);
    for(int i = 0; i <= horizontal; i ++)
    {
        qreal swidth = w/horizontal;
        if(i != horizontal)
        {
            for(int j = 0; j < 10; j++)
            {
                qreal x = (swidth/10 * j) + swidth * i;
                p.drawLine(x,h-5,x,h);
            }
        }
        p.drawLine(swidth * i,0,swidth * i,h);

        p.setPen(Qt::black);
        qreal scale = i;
        scale = scale/(horizontal/sec);
        p.drawText(swidth * i-2,h+10, QString("%1").arg(scale));
        p.setPen(Qt::gray);
    }

    for(int i = 0; i <= vertical; i ++)
    {
        p.drawLine(0,h/vertical * i, w,h/vertical * i);
    }
    p.setPen(Qt::black);


    p.setRenderHint(QPainter::Antialiasing);

    // отрисовываем сам график исходя из позиционных координат

    // добавляем новую стартовую позицию (крайний левый верхний угол линии)
    qreal Y = pin->level() ? HIGH : LOW;
    positions.append(QPointF(0, Y));

    // соединяем все точки
    for(int i = 0; i < positions.size(); i++)
    {
        // текущая и следующая точки
        QPointF current = positions.at(i);
        QPointF next = i+1 < positions.size() ? positions.at(i+1) : QPointF(0, Y);

        // если произошел переход через фронт - нарисовать вертикальную линию фронта и переместиться на Y координату фронта
        if(current.ry() != next.ry())
        {
            QPointF front =  QPointF(current.rx(), current.ry() == HIGH? LOW : HIGH);
            p.drawLine(current, front);
            current = front;
        }
        p.drawLine(current,next);
    }
    e->accept();
}

void MonitorWidget::timerEvent(QTimerEvent *)
{
    repaint();
}






















