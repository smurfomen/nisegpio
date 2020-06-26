#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    InitInpOut();
    if(!libloaded)
    {
        QMessageBox::warning(nullptr,"ОШИБКА", "Ошибка загрузки библиотеки inpout32.dll.\nПопробуйте перезапустить программу с правами администратора.");
        exit(0);
    }
    else
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
}
