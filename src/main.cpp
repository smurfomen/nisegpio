#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(InitInpOut())
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    else
    {
        QMessageBox::warning(nullptr,"ОШИБКА", "Ошибка загрузки библиотеки inpout32.dll.\nПопробуйте перезапустить программу с правами администратора.");
        return 0;
    }
}
