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
    MOVING
};

class GridPainter : public QOpenGLWidget
{
    Q_OBJECT
private:
    Grid grid;
    bool stopped;
    
    QColor cellColor;
    QColor spaceColor;
    
    QBrush cellBrush;
    QBrush spaceBrush;
    
    QPen cellPen;

    float scale;

    QPoint drawingPosition;
    QPoint mousePosition;

    bool mousePressed;

    MOUSE_MODE mode;

public:
    GridPainter(QWidget *parent);
    void setCellColor(QColor cc);
    void setSpaceColor(QColor sc);
    void initEmptyGrid(int width, int height);
    void parsePlainText(const QString &fileName);
    void setMouseMode(MOUSE_MODE m);

public slots:
    void animate();
    void stopPressed();

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

