#include "gridpainter.h"

#include <QPainter>
#include <QPaintEvent>

GridPainter::GridPainter(QWidget *parent) : QOpenGLWidget(parent)
{
    stopped = false;
    cellColor = Qt::green;
    spaceColor = Qt::white;
}

void GridPainter::animate()
{
    if(!stopped)
    {
        grid.update();
        update();
    }
}

void GridPainter::stopPressed()
{
    stopped = !stopped;
}

void GridPainter::setCellColor(QColor cc)
{
    cellColor = cc;
}

void GridPainter::setSpaceColor(QColor sc)
{
    spaceColor = sc;
}

void GridPainter::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter;
    painter->begin(this);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::Dense2Pattern);
    painter->setPen(QPen(cellColor));
    painter->setBackground(QBrush(spaceColor));
    painter->translate(event->rect().width() / 2, event->rect().height() / 2);
    painter->drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("Qt"));
    painter->end();
}
