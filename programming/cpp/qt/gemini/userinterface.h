#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMainWindow>

#include "gridpainter.h"

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
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
class QTreeView;
class QWidget;
QT_END_NAMESPACE

class UserInterface : public QMainWindow
{
    Q_OBJECT

protected:
    //void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void setCellColor();
    void setSpaceColor();
    void openFile();
    void initRandom();
    void changeMode(const QModelIndex & index);
    void stopButtonPressed();

private:
    void createActions();
    void createMenus();
    QAbstractItemModel *modelFromFile(const QString& fileName);

    QWidget *all; // All widgets belong to this
    GridPainter *gridPainter; // A widget with cells
    QPushButton *stopButton;
    QPushButton *clearButton;
    QPushButton *rotateClockwiseButton;
    QPushButton *rotateAntiClockwiseButton;
    QPushButton *nextGenerationButton;
    QMenu *viewMenu;
    QMenu *fileMenu;
    QMenuBar *menuBar; // the entire menu strip
    QAction *setCellColorAct;
    QAction *setSpaceColorAct;
    QAction *openFileAct;
    QAction *initRandomAct;
    QTimer *timer; // timer that calls gridPainter.animate()s
    QVBoxLayout *layout; // contains mainLayout and painterAndMode
    QHBoxLayout *mainLayout; // contains buttons at the bottom
    QTreeView *mode; // specifies a drawing/erasing pattern and mode
    QHBoxLayout *painterAndMode; // contains gridPainter and mode

    int drawingIndex; // index of "drawing" in treeView
    int erasingIndex; // index of "erasing" in treeView

public:
    UserInterface();
    ~UserInterface();
};

#endif // USERINTERFACE_H
