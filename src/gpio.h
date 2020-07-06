#ifndef GPIO_H
#define GPIO_H
#include <QtCore>

namespace gpio{
int inp(int port);

void outp(int port, short value);
}
int IsInpOutDriverOpen();
bool InitInpOut();


int InpPort(short port);
void OutPort (short port, short value);
extern bool libloaded;

#endif // GPIO_H
