#ifndef GRIDPAINTER
#define GRIDPAINTER

#include <QOpenGLWidget>
#include <QPen>

#include "grid.h"

QT_BEGIN_NAMESPACE
class QColor;
class QPaintEvent;
class QWidget;
class QPen;
class QBrush;
QT_END_NAMESPACE

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

public:
    GridPainter(QWidget *parent);
    void setCellColor(QColor cc);
    void setSpaceColor(QColor sc);

public slots:
    void animate();
    void stopPressed();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // GRIDPAINTER

