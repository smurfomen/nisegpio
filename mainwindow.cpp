#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidget>
#include <QMessageBox>
QList<Pin*> Pin::plist;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("NISE-105 GPIO Viewer");

    Pin::declare(0xA07, 1, "GPO3");
    Pin::declare(0xA07, 0, "GPO2");
    Pin::declare(0xA02, 5, "GPO1");
    Pin::declare(0xA03, 6, "GPO0");
    Pin::declare(0xA00, 1, "GPI3");
    Pin::declare(0xA05, 4, "GPI2");
    Pin::declare(0xA05, 5, "GPI1");
    Pin::declare(0xA03, 1, "GPI0");

    ui->gpo0->setChecked(Pin::byName("GPO0")->level());
    ui->gpo1->setChecked(Pin::byName("GPO1")->level());
    ui->gpo2->setChecked(Pin::byName("GPO2")->level());
    ui->gpo3->setChecked(Pin::byName("GPO3")->level());


    ui->notation->addItem("HEX", QVariant::fromValue(16));
    ui->notation->addItem("DEC", QVariant::fromValue(10));
    ui->notation->addItem("BIN", QVariant::fromValue(2));

    // Наполняем комбобоксы и привязываем их к существующим пинам
    for(int i = 0; i < 4; i ++)
    {
        Pin * pin = Pin::byName(QString("GPI%1").arg(i));

        ui->monitorPin->addItem(pin->name);
        ui->monitorPin->setItemData(ui->monitorPin->count()-1, QVariant::fromValue((void *) pin), Qt::UserRole);
    }

    for(int i = 0; i < 4; i ++)
    {
        Pin * pin = Pin::byName(QString("GPO%1").arg(i));

        ui->implPin->addItem(pin->name);
        ui->implPin->setItemData(ui->implPin->count()-1, QVariant::fromValue((void *)pin), Qt::UserRole);

        ui->pulsePin->addItem(pin->name);
        ui->pulsePin->setItemData(ui->pulsePin->count()-1, QVariant::fromValue((void *)pin), Qt::UserRole);

        ui->monitorPin->addItem(pin->name);
        ui->monitorPin->setItemData(ui->monitorPin->count()-1, QVariant::fromValue((void *) pin), Qt::UserRole);
    }

    startTimer(50);

    QTableWidget * t = ui->tableWidget;
    QStringList header;
    header <<"NAME"<<"ADDR"<<"D"<<"LEVEL"<<"STATE";
    t->setColumnCount(header.size());
    t->setHorizontalHeaderLabels(header);

    for(auto pin : Pin::plist)
    {
        addPinItem(pin);
    }

    pulseTmr = new QTimer();
    connect(pulseTmr, SIGNAL(timeout()), this, SLOT(pulseTime()));
}


MainWindow::~MainWindow()
{
    for(auto pin : Pin::plist)
    {
        delete pin;
    }
    Pin::plist.clear();
    delete pulseTmr;
    delete ui;
}


void MainWindow::updatePinItem(Pin * pin)
{
    QTableWidget * t = ui->tableWidget;

    for(int i = 0; i < t->rowCount(); i++)
    {
        if((Pin*)t->item(i,0)->data(Qt::UserRole).value<void*>() == pin)
        {
            t->item(i,3)->setText(pin->level()? "HIGH" : "LOW");
            QString pv = QString::number(pin->state(),2);
            pv.prepend(QByteArray(8-pv.length(), '0'));
            t->item(i,4)->setText(pv);
        }
    }
}

void MainWindow::addPinItem(Pin * pin)
{
    QTableWidget * t = ui->tableWidget;
    t->insertRow(0);
    t->setItem(0,0, new QTableWidgetItem(pin->name));
    t->setItem(0,1, new QTableWidgetItem("0x"+QString::number(pin->addr,16).toUpper()));
    t->setItem(0,2, new QTableWidgetItem(QString::number(pin->bit)));
    t->setItem(0,3, new QTableWidgetItem());
    t->setItem(0,4, new QTableWidgetItem());
    t->item(0,0)->setData(Qt::UserRole, QVariant::fromValue((void*)pin));
}

void MainWindow::on_gpo0_clicked(bool checked)
{
    ui->gpo0->setChecked(checked);
    Pin::byName("GPO0")->Out(checked);
}

void MainWindow::on_gpo1_clicked(bool checked)
{
    ui->gpo1->setChecked(checked);
    Pin::byName("GPO1")->Out(checked);

}

void MainWindow::on_gpo2_clicked(bool checked)
{
    ui->gpo2->setChecked(checked);
    Pin::byName("GPO2")->Out(checked);
}

void MainWindow::on_gpo3_clicked(bool checked)
{
    ui->gpo3->setChecked(checked);
    Pin::byName("GPO3")->Out(checked);
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    ui->gpi0led->setColor(!Pin::byName("GPI0")->level() ? Qt::gray : Qt::green);
    ui->gpi0->setNum(Pin::byName("GPI0")->level() ? 1 : 0);

    ui->gpi1led->setColor(!Pin::byName("GPI1")->level() ? Qt::gray : Qt::green);
    ui->gpi1->setNum(Pin::byName("GPI1")->level() ? 1 : 0);

    ui->gpi2led->setColor(!Pin::byName("GPI2")->level()? Qt::gray : Qt::green);
    ui->gpi2->setNum(Pin::byName("GPI2")->level() ? 1 : 0);

    ui->gpi3led->setColor(!Pin::byName("GPI3")->level() ? Qt::gray : Qt::green);
    ui->gpi3->setNum(Pin::byName("GPI3")->level() ? 1 : 0);

    ui->gpo0->setChecked(Pin::byName("GPO0")->level());
    ui->gpo1->setChecked(Pin::byName("GPO1")->level());
    ui->gpo2->setChecked(Pin::byName("GPO2")->level());
    ui->gpo3->setChecked(Pin::byName("GPO3")->level());

    for(auto pin : Pin::plist)
    {
        updatePinItem(pin);
    }

    e->accept();
}

void MainWindow::on_putimpl_clicked()
{
    switch (ui->implPin->currentIndex()) {
    case 0 :
        on_gpo0_clicked(true);
        QTimer::singleShot(ui->implWidth->value()*1000,[this]{on_gpo0_clicked(false);});
        break;
    case 1:
        on_gpo1_clicked(true);
        QTimer::singleShot(ui->implWidth->value()*1000,[this]{on_gpo1_clicked(false);});
        break;
    case 2:
        on_gpo2_clicked(true);
        QTimer::singleShot(ui->implWidth->value()*1000,[this]{on_gpo2_clicked(false);});
        break;
    case 3 :
        on_gpo3_clicked(true);
        QTimer::singleShot(ui->implWidth->value()*1000,[this]{on_gpo3_clicked(false);});
        break;
    default:
        return;
    }
}


void MainWindow::on_pulseEnable_clicked(bool checked)
{
    if(checked)
    {
        pulseTmr->start(ui->pulseRange->value() * 1000);
        pulseTime();
    }
    else
        pulseTmr->stop();
}

void MainWindow::pulseTime()
{
    Pin * p = (Pin*) ui->pulsePin->itemData(ui->pulsePin->currentIndex(), Qt::UserRole).value<void*>();
    p->Out(!p->level());
}


void MainWindow::on_pulseRange_valueChanged(double arg1)
{
    if(pulseTmr->isActive())
    {
        pulseTmr->start(arg1*1000);
    }
    else
        pulseTmr->setInterval(arg1*1000);
}


void MainWindow::on_openMonitor_clicked()
{
    Pin * p = (Pin*) ui->monitorPin->itemData(ui->monitorPin->currentIndex(), Qt::UserRole).value<void*>();
    PinMonitor * monitor = new PinMonitor(p, this);
    monitor->setWindowFlag(Qt::Dialog);
    monitor->show();
}


void MainWindow::on_output_clicked()
{
    QString port = ui->customWAddr->text();
    port = port.trimmed();
    port.replace("0x","");
    if(!port.isEmpty() && port != "0")
        gpio::outp(port.toShort(nullptr, 16), ui->customWVal->text().toShort(nullptr,16));
}

void MainWindow::on_input_clicked()
{
    if(!ui->customRAddr->text().isEmpty())
    {
        QString port = ui->customRAddr->text();
        port = port.trimmed();
        port.replace("0x","");
        if(!port.isEmpty() && port != "0")
        {
            int notation = ui->notation->currentData(Qt::UserRole).toInt();
            QString customReadValue = QString::number(gpio::inp(port.toShort(nullptr,16)),notation);

            // дополнить нулями если значение в 2 системе и заняты не все биты
            if(notation == 2)
                customReadValue.prepend(QByteArray(8-customReadValue.length(), '0'));

            // дополнить нулем если значение в 16 системе и не заняты старшие 4 бита
            else if(notation == 16)
                customReadValue.prepend(QByteArray(2-customReadValue.length(), '0'));

            ui->customRVal->setText(customReadValue.toUpper());
        }
    }
    else ui->customRVal->clear();
}

void MainWindow::on_notation_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    on_input_clicked();
}













