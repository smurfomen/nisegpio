#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>
#include <QFrame>
namespace Ui {
class MonitorWidget;
}

class MonitorWidget : public QFrame
{
    Q_OBJECT

public:
    explicit MonitorWidget(QWidget *parent = nullptr);
    ~MonitorWidget();

    void setPin(class Pin* pin);

    void setTimeScale(quint8 sec);

    virtual void paintEvent(QPaintEvent *) override;
    virtual void timerEvent(QTimerEvent *) override;

private:

    int sec = 5;
    int vertical = 10;      // вертикальных линий
    int horizontal = 10;    // горизонтальных линий
    qreal w = 300;
    qreal h = 100;

    Ui::MonitorWidget *ui;
    class Pin * pin {nullptr};
    QList<QPointF> positions;
};

#endif // MONITORWIDGET_H
