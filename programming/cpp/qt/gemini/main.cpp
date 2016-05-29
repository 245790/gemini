/* KPCC
 * Entry point of the application
 * File: main.cpp
 * Date: 2015.09.05
 */
#include <QApplication>

#include "userinterface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UserInterface w;
    w.show();

    return a.exec();
}
