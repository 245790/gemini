#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMainWindow>

#include "gridpainter.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QMenu;
class QMenuBar;
class QVBoxLayout;
class QHBoxLayout;
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
    void openFile();
    void initRandom();
    void changeMode(QListWidgetItem *current, QListWidgetItem *previous);
    void stopButtonPressed();

private:
    void createActions();
    void createMenus();
    void createIcons();

    QWidget *all;
    GridPainter *gridPainter;
    QPushButton *stopButton;
    QPushButton *clearButton;
    QPushButton *rotateClockwiseButton;
    QPushButton *rotateAntiClockwiseButton;
    QPushButton *nextGenerationButton;
    QMenu *viewMenu;
    QMenu *fileMenu;
    QMenuBar *menuBar;
    QAction *setCellColorAct;
    QAction *setSpaceColorAct;
    QAction *openFileAct;
    QAction *initRandomAct;
    QTimer *timer;
    QVBoxLayout *layout;
    QHBoxLayout *mainLayout;
    QListWidget *mode;
    QListWidgetItem *drawing;
    QListWidgetItem *moving;
    QHBoxLayout *painterAndMode;

public:
    UserInterface();
    ~UserInterface();
};

#endif // USERINTERFACE_H
