/* KPCC
 * GridPainter is a widget able to draw Grids
 * Author: Safin Karim, Alexandra Balyuk
 * Date: 2015.09.05
 */

#ifndef GRIDPAINTER
#define GRIDPAINTER

#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPen>
#include <QVector>
#include <QWheelEvent>

#include "grid.h"

QT_BEGIN_NAMESPACE
class QColor;
class QPaintEvent;
class QWidget;
class QPen;
class QBrush;
QT_END_NAMESPACE

enum MOUSE_MODE
{
    DRAWING,
    MOVING,
    ERASING
};

class GridPainter : public QOpenGLWidget
{
    Q_OBJECT
private:
    Grid grid;

    QVector<Grid> painting; // patterns that we paint with
    int currentPaintingIndex; // exact pattern used for drawing
    QVector<Grid> erasing; // patterns that we erase with
    int currentErasingIndex; // exact pattern used for drawing

    bool stopped; // true if the field is updating
    
    QColor cellColor; // color of a living cell
    QColor spaceColor; // color of a dead cel;
    
    QBrush cellBrush;
    QBrush spaceBrush;
    
    QPen cellPen;

    float scale; // 0.25 means that a cell is 1 pixel wide; cannot be less

    int cellWidth; // width of a cell, pixels

    QPoint drawingPosition; // center of a field. It is being drawn "around" this point
    QPoint mousePosition; // position of a mouse; we need to store it so as to draw/move

    bool mousePressed;

    MOUSE_MODE mode; // how we process mouse events

public:
    GridPainter(QWidget *parent);
    void setCellColor(QColor cc);
    void setSpaceColor(QColor sc);

    // creates a square grid, whose side is bigger that width and height
    void initEmptyGrid(int width, int height);

    void parsePlainText(const QString &fileName);
    void parseRLE(const QString &fileName);
    void setMouseMode(MOUSE_MODE m);
    void initRandom(int width, int height);
    bool isStopped();

    // These two parse text files and put that in corresponding Grids
    void setDrawingPattern(int index, const QString& fileName);
    void setErasingPattern(int index, const QString& fileName);
    // sets a pattern, no matter the mode. Used by UserInterface::keyPressEvent
    void setCurrentPattern(int index);
    void setCurrentDrawingPattern(int index);
    void setCurrentErasingPattern(int index);

public slots:
    void animate();
    void stopPressed();
    void clear();
    void rotateClockwise();
    void rotateAntiClockwise();
    void nextGeneration();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // GRIDPAINTER

