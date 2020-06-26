#ifndef QLED_H
#define QLED_H

#include "QPainter"
#include "QLabel"
#include "QColor"
#include "QBrush"
#include "QMouseEvent"
#include "QToolTip"

// класс индикатора прямоугольной или овальной формы, возможно с текстом, с возможностью мигания
class QLed : public QLabel
{
    Q_OBJECT
public:

    // перечисление возможных состояний
    enum ledStatus
    {
        on,                                                 // вкл
        off,                                                // выкл
        hidden,                                             // скрыт
        blink,                                              // мигает
        blink_once,                                         // игает 1 раз
    };

    // перечисление возможных форм
    enum ledShape
    {
        round,                                              // овал
        box,                                                // прямоугольник
    };


    QLed(QWidget * p,ledShape shape=round, ledStatus status=on, QBrush fore=Qt::green, QBrush back = Qt::gray);
    void set (ledShape shape=round, ledStatus = on, QBrush fore=Qt::green, QBrush back = Qt::gray);
    void setColor(QBrush clr);
    void setStatus(ledStatus sts);
    void setText (QString value) { text = value; }          // установка/изменение текста надписи
    void setTextColor (QColor value) { textColor = value; } // цвет текста
    void setBorderColor (QColor value) { borderColor = value; }// цвет окантовки
    void setToolTipMode (bool s) { setMouseTracking(s); }   // показ/отмена тултипов
    QBrush Fore () { return fore; }                         // актуальный цвет переднего плана
    QBrush Back () { return back; }                         // актуальный цвет заднего плана
signals:
    void ledClicked (QLed *);                               // сигнал щелчок мышью на индикаторе отправляется подписавшимся виджетам

protected:
virtual void timerEvent(QTimerEvent *event);                // таймер
virtual void paintEvent(QPaintEvent* e);                    // отрисовка
virtual void mousePressEvent(QMouseEvent * event);          // клик мыши
virtual void mouseMoveEvent(QMouseEvent *);                 // перемещение мыши (для тултипа)

private:
    ledShape  shape;                                        // форма
    ledStatus status;                                       // состояние
    int       timerId;                                      // таймер
    bool      pulse;                                        // флаг пульсации
    QBrush    fore;                                         // актуальный цвет
    QBrush    back;                                         // цвет фона при мигании

    QString   text;                                         // текст надписи
    QColor    textColor;                                    // цвет текста
    QColor    borderColor;                                  // цвет окантовки
};

#endif // QLED_H
