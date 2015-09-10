#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "gridpainter.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QMenuBar;
class QVBoxLayout;
class QPushButton;
class QTimer;
class QWidget;
QT_END_NAMESPACE

class UserInterface : public QMainWindow
{
    Q_OBJECT

private slots:
    void setCellColor();
    void setSpaceColor();

private:
    void createActions();
    void createMenus();

    GridPainter *gridPainter;
    QPushButton *stopButton;
    QMenu *viewMenu;
    QMenuBar *menuBar;
    QAction *setCellColorAct;
    QAction *setSpaceColorAct;
    QTimer *timer;
    QVBoxLayout *mainLayout;
    QWidget *all;

public:
    UserInterface();
    ~UserInterface();
};

#endif // USERINTERFACE_H
