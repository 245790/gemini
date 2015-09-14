#include "gridpainter.h"

#include <QPainter>
#include <QPaintEvent>

#include <QtWidgets>
#include <QWidget>

GridPainter::GridPainter(QWidget *parent) : QOpenGLWidget(parent)
{
    stopped = false;
    
    cellColor = QColor(147, 161, 161);
    spaceColor = QColor(0, 43, 54);

    cellPen = QPen(cellColor);
    cellPen.setStyle(Qt::NoPen);
    
    cellBrush.setColor(QColor(147, 161, 161));
    cellBrush.setStyle(Qt::SolidPattern);

    spaceBrush.setColor(QColor(0, 43, 54));
    spaceBrush.setStyle(Qt::SolidPattern);
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

    cellBrush.setColor(cellColor);
    cellPen.setColor(cellColor);
}

void GridPainter::setSpaceColor(QColor sc)
{
    spaceColor = sc;

    spaceBrush.setColor(spaceColor);
}

void GridPainter::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter;
    painter->begin(this);
    
    painter->fillRect(event->rect(), Qt::white);
    painter->setBrush(cellBrush);
    painter->setPen(cellPen);

    float screenRatio = event->rect().width() / event->rect().height();
    float gridRatio = grid.getWidth() / grid.getHeight();

    float cellWidth = 0;

    if(screenRatio > gridRatio)
    {
        cellWidth = event->rect().height() / grid.getHeight();
        if(grid.getWidth() * cellWidth > event->rect().width())
        {
            cellWidth = event->rect().width() / grid.getWidth();
        }
        if(grid.getHeight() * cellWidth > event->rect().height())
        {
            cellWidth = event->rect().height() / grid.getHeight();
        }
//        painter->translate((event->rect().width() - grid.getWidth() * cellWidth) / 2, 0);
        painter->fillRect(QRectF(0,
                                 0,
                                 grid.getWidth() * cellWidth,
                                 grid.getHeight() * cellWidth),
                          spaceBrush);
    }
    else
    {
        cellWidth = event->rect().width() / grid.getWidth();
        if(grid.getWidth() * cellWidth > event->rect().width())
        {
            cellWidth = event->rect().width() / grid.getWidth();
        }
        if(grid.getHeight() * cellWidth > event->rect().height())
        {
            cellWidth = event->rect().height() / grid.getHeight();
        }
//        painter->translate(0, (event->rect().height() - grid.getHeight() * cellWidth)/ 2);
        painter->fillRect(QRectF(0,
                                 0,
                                 grid.getWidth() * cellWidth,
                                 grid.getHeight() * cellWidth),
                          spaceBrush);
    }

    int gridWidth = grid.getWidth();
    int gridHeight = grid.getHeight();

    painter->save();
    for(int i = 0; i < gridWidth; i++)
    {
        for(int j = 0; j < gridHeight; j++)
        {
            if(grid.isAlive(i, j))
            {
                painter->drawRect(QRectF(i * cellWidth,
                                         j * cellWidth,
                                         cellWidth,
                                         cellWidth));
            }
        }
    }
    painter->restore();

    painter->end();
}
