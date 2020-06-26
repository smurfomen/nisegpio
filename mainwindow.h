#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "gpio.h"
#include "pinmonitor.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Pin {
public:

    static Pin * byName(QString name)
    {
        for(auto pin : plist)
        {
            if(name == pin->name)
                return pin;
        }
        throw std::out_of_range(QString("Not found pin %1").arg(name).toStdString().c_str());
    }

    static void declare(short addr, quint8 bit, QString name)
    {
        plist.append(new Pin(addr,bit,name));
    }

    static QList<Pin*> plist;


    uchar state() {
        addrState = gpio::inp(addr);
        return addrState;
    }

    bool level() {
        pinLevel = (state() & (0x01 << bit)) > 0;
        return pinLevel;
    }

    void Out(bool levelHigh) {
        uchar s = state();
        uchar val = levelHigh ? s | (0x01 << bit) : s & ~(0x01 << bit);
        gpio::outp(addr, val);
    }



    Pin(short addr, quint8 bit, QString name){
        this->addr = addr;
        this->bit = bit;
        this->name = name;
    }

    short addr;
    quint8 bit;
    QString name;
    uchar addrState;
    bool pinLevel;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_gpo0_clicked(bool checked);

    void on_gpo1_clicked(bool checked);

    void on_gpo2_clicked(bool checked);

    void on_gpo3_clicked(bool checked);

    void on_putimpl_clicked();

    void on_pulseEnable_clicked(bool checked);

    void pulseTime();

    void on_pulseRange_valueChanged(double arg1);

    void on_openMonitor_clicked();

    void on_output_clicked();

    void on_input_clicked();

    void on_notation_currentIndexChanged(int index);

private:
    void updatePinItem(Pin *pin);
    void addPinItem(Pin *pin);
    virtual void timerEvent(QTimerEvent * e);
    QTimer * pulseTmr{nullptr};
    Ui::MainWindow *ui;
    QList<PinMonitor*> monitors;
};
#endif // MAINWINDOW_H
