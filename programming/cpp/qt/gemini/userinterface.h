/* KPCC
 * Graphical user interface
 * Author: Alexandra Balyuk
 * Date: 2015.09.05
 */

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QAbstractItemModel>
#include <QAction>
#include <QActionGroup>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QTreeView>
#include <QWidget>

#include "gridpainter.h"
#include "propertieswindow.h"

class UserInterface : public QMainWindow
{
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void setCellColor();
    void setSpaceColor();
    void setGridColor();
    void chooseWhiteTheme();
    void chooseBlackTheme();
    void fitPattern();
    void setUpdateRate();
    void openRleFile();
    void openPlainTextFile();
    void saveAsRleFile();
    void saveAsPlainTextFile();
    void initRandom();
    void changeMode(const QModelIndex & index);
    void stopButtonPressed();
    void updatePropertiesWindow();

private:
    void createActions();
    void createMenus();
    QAbstractItemModel *modelFromFile(const QString& fileName);

    QWidget *all; // All widgets belong to this
    GridPainter *gridPainter; // A widget with cells
    QPushButton *stopButton;
    QPushButton *clearButton;
    QPushButton *nextGenerationButton;
    QMenu *viewMenu;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenuBar *menuBar; // the entire menu strip
    QAction *setCellColorAct;
    QAction *setSpaceColorAct;
    QAction *setGridColorAct;
    QAction *chooseWhiteThemeAct;
    QAction *chooseBlackThemeAct;
    QAction *fitPatternAct;
    QAction *setUpdateRateAct;
    QAction *initRandomAct;
    QAction *openRleFileAct;
    QAction *openPlainTextFileAct;
    QAction *saveAsRleFileAct;
    QAction *saveAsPlainTextFileAct;
    QAction *rotateClockwiseAct;
    QAction *rotateAntiClockwiseAct;
    QTimer *timer; // Calls gridPainter::animate() and
                   // userInterface::updatePropertiesWindow() repeatedly
    QVBoxLayout *layout; // contains mainLayout and painterAndMode
    QHBoxLayout *mainLayout; // contains buttons at the bottom
    QTreeView *mode; // specifies a drawing/erasing pattern and mode
    QHBoxLayout *painterAndMode; // contains gridPainter and mode
    PropertiesWindow *propertiesWindow; // Shows various information about
                                        // the automata

    int drawingIndex; // index of "drawing" in treeView
    int erasingIndex; // index of "erasing" in treeView

public:
    UserInterface();
    ~UserInterface();
};

#endif // USERINTERFACE_H
