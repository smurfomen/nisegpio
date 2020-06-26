#ifndef PINMONITOR_H
#define PINMONITOR_H

#include <QWidget>

namespace Ui {
class PinMonitor;
}

class PinMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit PinMonitor(class Pin *pin, QWidget *parent = nullptr);
    ~PinMonitor();
    virtual void paintEvent(QPaintEvent *event) override;

private slots:
    void on_timeScale_valueChanged(int arg1);

private:
    Ui::PinMonitor *ui;
    class Pin * pin {nullptr};
};

#endif // PINMONITOR_H
