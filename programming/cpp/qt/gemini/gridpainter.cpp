/* KPCC
 * GridPainter is a widget able to draw Grids
 * Author: Safin Karim, Alexandra Balyuk
 * Date: 2015.09.05
 */

#include <fstream>
#include <string>
#include <QApplication>
#include <QPaintEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QtWidgets>
#include <QWidget>

#include "gridpainter.h"

GridPainter::GridPainter(QWidget *parent) : QOpenGLWidget(parent)
{
    stopped = true;

    mousePressed = false;

    scale = 1;

    drawingPosition = QPoint(4 * scale * grid.getWidth() / 2, 4 * scale * grid.getHeight() / 2);

    cellColor = QColor(0, 0, 0);
    spaceColor = QColor(240, 240, 240);

    cellPen = QPen(cellColor);
    cellPen.setStyle(Qt::NoPen);
    //cellPen.setStyle(Qt::SolidLine);
    //cellPen.setJoinStyle(Qt::MiterJoin);
    
    cellBrush.setColor(QColor(0, 0, 0));
    cellBrush.setStyle(Qt::SolidPattern);

    spaceBrush.setColor(QColor(250, 250, 250));
    spaceBrush.setStyle(Qt::SolidPattern);

    setMouseMode(MOVING);

    setFocusPolicy(Qt::ClickFocus);

    currentPaintingIndex = 0;
    currentErasingIndex = 0;
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

void GridPainter::clear()
{
    grid.clear();
    update();
}

void GridPainter::rotateClockwise()
{
    grid.rotateClockwise();
    update();
}

void GridPainter::rotateAntiClockwise()
{
    grid.rotateAntiClockwise();
    update();
}

void GridPainter::nextGeneration()
{
    if(stopped)
    {
        grid.update();
        update();
    }
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

void GridPainter::initEmptyGrid(int width, int height)
{
    grid.initEmptyGrid(width, height);
}

void GridPainter::parsePlainText(const QString &fileName)
{
    stopped = true;
    grid.parsePlainText(fileName);
    drawingPosition.setX(4 * scale * grid.getWidth() / 2);
    drawingPosition.setY(4 * scale * grid.getHeight() / 2);
}

void GridPainter::parseRLE(const QString &fileName)
{
    stopped = true;
    grid.parseRLE(fileName);
    drawingPosition.setX(4 * scale * grid.getWidth() / 2);
    drawingPosition.setY(4 * scale * grid.getHeight() / 2);
}

void GridPainter::initRandom(int width, int height)
{
    stopped = true;
    grid.clear();
    grid.initRandom(width, height);
    drawingPosition.setX(4 * scale * grid.getWidth() / 2);
    drawingPosition.setY(4 * scale * grid.getHeight() / 2);
}

bool GridPainter::isStopped()
{
    return stopped;
}

void GridPainter::setDrawingPattern(int index, const QString& fileName)
{
    if(index > 10 || index < 0) // we do not store more than 10
    {
        return;
    }
    if(index >= painting.size())
    {
        painting.resize(index + 1);
    }
    painting[index].parseRLE(fileName);
}

void GridPainter::setErasingPattern(int index, const QString& fileName)
{
    if(index > 10 || index < 0) // we do not store more than 10
    {
        return;
    }
    if(index >= erasing.size())
    {
        erasing.resize(index + 1);
    }
    erasing[index].parseRLE(fileName);
}

void GridPainter::setCurrentPattern(int index)
{
    if(mode == DRAWING)
    {
        setCurrentDrawingPattern(index);
    }
    if(mode == ERASING)
    {
        setCurrentErasingPattern(index);
    }
}

void GridPainter::setCurrentDrawingPattern(int index)
{
    if(painting.size() > index)
    {
        currentPaintingIndex = index;
    }
}

void GridPainter::setCurrentErasingPattern(int index)
{
    if(erasing.size() > index)
    {
        currentErasingIndex = index;
    }
}

void GridPainter::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter;
    painter->begin(this);
    
    painter->fillRect(event->rect(), Qt::white);
    painter->setBrush(cellBrush);
    painter->setPen(cellPen);

    cellWidth = 4 * scale;

    int width = grid.getWidth() * cellWidth;

    painter->fillRect(drawingPosition.x() - width / 2, drawingPosition.y() - width / 2, width, width, spaceBrush);

    painter->save();
    grid.draw(painter, drawingPosition.x(), drawingPosition.y(), grid.getWidth() * cellWidth);
    // draw a cell near a mouse
    /*painter->fillRect((mousePosition.x() - drawingPosition.x()) / cellWidth,
                      (mousePosition.y() - drawingPosition.y()) / cellWidth,
                      cellWidth,
                      cellWidth,
                      Qt::red);*/
    QFont f;
    f.setPixelSize(15);
    painter->setPen(Qt::SolidLine);
    painter->setFont(f);
    painter->drawText(QRect(0, 0, 1000, 100),
                      Qt::AlignLeft,
                      QStringLiteral("Generation ") + QString::number(grid.getGeneration()));
    painter->restore();

    painter->end();
}

void GridPainter::setMouseMode(MOUSE_MODE m)
{
    mode = m;
}

#ifndef QT_NO_WHEELEVENT
void GridPainter::wheelEvent(QWheelEvent *event)
{
    switch(mode)
    {
    case MOVING:
    case DRAWING:
    case ERASING:
        update();
        if (event->delta() > 0)
        {

            if (event->orientation() == Qt::Vertical)
            {
                scale *= 1.1f;
            }
        }
        else
        {
            if (event->delta() < 0)
            {
                if (event->orientation() == Qt::Vertical)
                {
                    if(scale / 1.1 > 0.25)
                    {
                        scale /= 1.1f;
                    }
                }
            }
        }
    break;
    break;
    }
}
#endif

void GridPainter::mousePressEvent(QMouseEvent *event)
{
    switch(mode)
    {
    case MOVING:
        if (event->button() == Qt::LeftButton)
        {
            update();
        }
    break;
    case DRAWING:
        if (event->button() == Qt::LeftButton)
        {
            update();
            int xPosition = (event->pos().x() - drawingPosition.x()) / cellWidth;
            int yPosition = (event->pos().y() - drawingPosition.y()) / cellWidth;
            // if the mouse is pressed, make the cells alive
            for(int i = 0; i < painting[currentPaintingIndex].getHeight(); i++)
            {
                for(int j = 0; j < painting[currentPaintingIndex].getWidth(); j++)
                {
                    if(painting[currentPaintingIndex].isAlive(i, j))
                    {
                        grid.setAlive(yPosition + j,
                                      xPosition + i,
                                      true);
                    }
                }
            }
        }
    break;
    case ERASING:
        if (event->button() == Qt::LeftButton)
        {
            update();
            int xPosition = (event->pos().x() - drawingPosition.x()) / cellWidth;
            int yPosition = (event->pos().y() - drawingPosition.y()) / cellWidth;
            // if the mouse is pressed, make the cells dead
            for(int i = 0; i < erasing[currentErasingIndex].getHeight(); i++)
            {
                for(int j = 0; j < erasing[currentErasingIndex].getWidth(); j++)
                {
                    if(erasing[currentErasingIndex].isAlive(i, j))
                    {
                        grid.setAlive(yPosition + j,
                                      xPosition + i,
                                      false);
                    }
                }
            }
        }
    break;
    }
    mousePosition.setX(event->pos().x());
    mousePosition.setY(event->pos().y());
}

void GridPainter::mouseMoveEvent(QMouseEvent *event)
{
    switch(mode)
    {
    case MOVING:
        if ((event->buttons() & Qt::LeftButton))
        {
            drawingPosition.setX(drawingPosition.x() + event->pos().x() - mousePosition.x());
            drawingPosition.setY(drawingPosition.y() + event->pos().y() - mousePosition.y());
        }
    break;
    case DRAWING:
        // it is easier to draw only on clicks
    break;
    case ERASING:
        /*if (event->button() == Qt::LeftButton)
        {*/
            update();
            int xPosition = (event->pos().x() - drawingPosition.x()) / cellWidth;
            int yPosition = (event->pos().y() - drawingPosition.y()) / cellWidth;
            // if the mouse is pressed, make the cells dead
            for(int i = 0; i < erasing[currentErasingIndex].getHeight(); i++)
            {
                for(int j = 0; j < erasing[currentErasingIndex].getWidth(); j++)
                {
                    if(erasing[currentErasingIndex].isAlive(i, j))
                    {
                        grid.setAlive(yPosition + j,
                                      xPosition + i,
                                      false);
                    }
                }
            }
        /*}*/
    break;
    }
    mousePosition.setX(event->pos().x());
    mousePosition.setY(event->pos().y());
    update();
}

void GridPainter::mouseReleaseEvent(QMouseEvent *event)
{
    mousePosition.setX(event->pos().x());
    mousePosition.setY(event->pos().y());
    if (event->buttons() & Qt::LeftButton)
    {
        drawingPosition.setX(drawingPosition.x() + event->pos().x() - mousePosition.x());
        drawingPosition.setY(drawingPosition.y() + event->pos().y() - mousePosition.y());
        update();
    }
}
