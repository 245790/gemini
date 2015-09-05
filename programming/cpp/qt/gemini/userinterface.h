#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QWidget>

class UserInterface : public QWidget
{
    Q_OBJECT

public:
    UserInterface(QWidget *parent = 0);
    ~UserInterface();
};

#endif // USERINTERFACE_H
