#include "gpio.h"
#include <QString>
#include <QDebug>
#include <windows.h>
#include <QLibrary>
typedef void	(__stdcall *lpOut32)(short, short);		// тип указатель на функцию вывода
typedef short	(__stdcall *lpInp32)(short);			// тип указатель на функцию ввода
typedef BOOL	(__stdcall *lpIsInpOutDriverOpen)(void);        // тип указатель на функцию готовности драйвера
typedef BOOL	(__stdcall *lpIsXP64Bit)(void);			// тип указатель на функцию проверки разрядности

//Some global function pointers (messy but fine for an example)
static lpOut32 gfpOut32                             = NULL;	// указатель на функцию вывода
static lpInp32 gfpInp32                             = NULL;	// указатель на функцию ввода
static lpIsInpOutDriverOpen gfpIsInpOutDriverOpen   = NULL;	// указатель на функцию готовности драйвера
static lpIsXP64Bit gfpIsXP64Bit                     = NULL;	// указатель на функцию проверки разрядности
bool libloaded = false;
QLibrary hInpOutDll;
#include <QDir>
#include <QFile>
void InitInpOut()
{

    QFile lib("inpout32.dll");
    QFile rec(":/lib/inpout32.dll");

    if(rec.open(QIODevice::ReadOnly))
    {
        if(lib.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            lib.write(rec.readAll());
            lib.close();

            hInpOutDll.setFileName(lib.fileName());

            if ((libloaded = hInpOutDll.load()))
            {
                gfpOut32                = (lpOut32)                 hInpOutDll.resolve("Out32");
                gfpInp32                = (lpInp32)                 hInpOutDll.resolve("Inp32");
                gfpIsInpOutDriverOpen   = (lpIsInpOutDriverOpen)    hInpOutDll.resolve("IsInpOutDriverOpen");
                gfpIsXP64Bit            = (lpIsXP64Bit)             hInpOutDll.resolve("IsXP64Bit");
            }
            else
            {
                hInpOutDll.setFileName(QDir::currentPath() + "\\inpout32.dll");
                if((libloaded = hInpOutDll.load()))
                {
                    gfpOut32                = (lpOut32)                 hInpOutDll.resolve("Out32");
                    gfpInp32                = (lpInp32)                 hInpOutDll.resolve("Inp32");
                    gfpIsInpOutDriverOpen   = (lpIsInpOutDriverOpen)    hInpOutDll.resolve("IsInpOutDriverOpen");
                    gfpIsXP64Bit            = (lpIsXP64Bit)             hInpOutDll.resolve("IsXP64Bit");
                }
            }
        }
        rec.close();
    }
}

int IsInpOutDriverOpen()
{
    return (gfpIsInpOutDriverOpen) ? gfpIsInpOutDriverOpen() : FALSE;
}

int InpPort(short port)
{
    short ret = 0;
    if (gfpIsInpOutDriverOpen && gfpIsInpOutDriverOpen())
    {
        ret = gfpInp32(port);
    }
    return ret;
}

void OutPort (short port, short value)
{
    if (gfpIsInpOutDriverOpen && gfpIsInpOutDriverOpen())
    {
        gfpOut32(port, value);
    }
}


int gpio::inp(int port)
{
    return InpPort(port);
}

void gpio::outp(int port, short value)
{
    OutPort(port,value);
}
