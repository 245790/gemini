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

    grid.initEmptyGrid(10, 10);

    topLeftDrawingPosition = QPoint(0, 0);
    bottomRightDrawingPosition = QPoint(100, 100);

    cellColor = QColor(0, 0, 0);
    spaceColor = QColor(240, 240, 240);
    gridColor = QColor(230, 230, 230);
    
    cellBrush.setColor(cellColor);
    cellBrush.setStyle(Qt::SolidPattern);

    gridPen.setColor(gridColor);
    gridPen.setStyle(Qt::SolidLine);

    mouseScrollSensitivity = 1.3;

    setMouseMode(MOVING);

    setFocusPolicy(Qt::ClickFocus);

    currentPaintingIndex = 0;
    currentErasingIndex = 0;
}

void GridPainter::autoFitDrawingPoints()
{
    topLeftDrawingPosition = QPoint(0, 0);
    // wholeWidth is such a width of a field that all the cells have whole width
    int wholeWidth;
    if(this->width() > this->height())
    {
        if(grid.getWidth() < this->width())
        {
            wholeWidth = this->height() / grid.getWidth() * grid.getWidth();
        }
        else
        {
            wholeWidth = grid.getWidth();
        }
    }
    else
    {
        if(grid.getWidth() < this->height())
        {
            wholeWidth = this->height() / grid.getWidth() * grid.getWidth();
        }
        else
        {
            wholeWidth = grid.getWidth();
        }
    }
    bottomRightDrawingPosition = QPoint(wholeWidth, wholeWidth);
}

void GridPainter::preventResizing(int prevGridWidth, int currentGridWidth, int prevFieldWidth)
{
    if(prevGridWidth != currentGridWidth) // prevent unneeded resizing
    {
        int prevWidth = bottomRightDrawingPosition .x() - topLeftDrawingPosition.x();
        if(prevGridWidth < currentGridWidth)
        {
            topLeftDrawingPosition.setX(topLeftDrawingPosition.x() -
                                        prevFieldWidth / 2);
            topLeftDrawingPosition.setY(topLeftDrawingPosition.y() -
                                        prevFieldWidth / 2);
            bottomRightDrawingPosition.setX(bottomRightDrawingPosition.x() +
                                            prevFieldWidth / 2);
            bottomRightDrawingPosition.setY(bottomRightDrawingPosition.y() +
                                            prevFieldWidth / 2);

        }
        else
        {
            topLeftDrawingPosition.setX(topLeftDrawingPosition.x() +
                                        prevFieldWidth / 4);
            topLeftDrawingPosition.setY(topLeftDrawingPosition.y() +
                                        prevFieldWidth / 4);
            bottomRightDrawingPosition.setX(bottomRightDrawingPosition.x() -
                                            prevWidth / 4);
            bottomRightDrawingPosition.setY(bottomRightDrawingPosition.y() -
                                            prevWidth / 4);

        }

    }
}

void GridPainter::animate()
{
    if(!stopped)
    {
        int prevGridWidth = grid.getWidth();
        grid.update();
        int currentGridWidth = grid.getWidth();
        preventResizing(prevGridWidth,
                        currentGridWidth,
                        bottomRightDrawingPosition.x() - topLeftDrawingPosition.x());

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
        int prevGridWidth = grid.getWidth();
        grid.update();
        int currentGridWidth = grid.getWidth();
        preventResizing(prevGridWidth,
                        currentGridWidth,
                        bottomRightDrawingPosition.x() - topLeftDrawingPosition.x());
        update();
    }
}

void GridPainter::setCellColor(QColor cc)
{
    cellColor = cc;

    cellBrush.setColor(cellColor);
}

void GridPainter::setSpaceColor(QColor sc)
{
    spaceColor = sc;
}

void GridPainter::setGridColor(QColor gc)
{
    gridColor = gc;

    gridPen.setColor(gridColor);
}

int GridPainter::getGenerationCount()
{
    return grid.getGeneration();
}

long GridPainter::getPopulation()
{
    return grid.getPopulation();
}

void GridPainter::initEmptyGrid(int width, int height)
{
    grid.initEmptyGrid(width, height);
}

void GridPainter::parsePlainText(const QString &fileName)
{
    stopped = true;
    grid.parsePlainText(fileName);
    autoFitDrawingPoints();
}

void GridPainter::parseRLE(const QString &fileName)
{
    stopped = true;
    grid.parseRLE(fileName);
    autoFitDrawingPoints();
}

void GridPainter::initRandom(int width, int height)
{
    stopped = true;
    grid.clear();
    grid.initRandom(width, height);
    autoFitDrawingPoints();
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

    painter->fillRect(QRect(topLeftDrawingPosition, bottomRightDrawingPosition), spaceColor);

    painter->setBrush(cellBrush);

    int fieldWidth = (bottomRightDrawingPosition.x() - topLeftDrawingPosition.x());

    painter->save();
    painter->setPen(Qt::NoPen);
    grid.draw(painter,
              topLeftDrawingPosition.x() + fieldWidth / 2,
              topLeftDrawingPosition.y() + fieldWidth / 2,
              fieldWidth);

    int cellWidth = fieldWidth / grid.getWidth();
    if(cellWidth > 3)
    {
        painter->setPen(gridPen);
        for(int i = topLeftDrawingPosition.x();
                i < bottomRightDrawingPosition.x();
                i += cellWidth)
        {
            painter->drawLine(i,
                              topLeftDrawingPosition.y(),
                              i,
                              bottomRightDrawingPosition.y());
        }
        for(int i = topLeftDrawingPosition.y();
                i < bottomRightDrawingPosition.y();
                i += cellWidth)
        {
            painter->drawLine(topLeftDrawingPosition.x(),
                              i,
                              bottomRightDrawingPosition.x(),
                              i);
        }
    }

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
        int mouseX = event->pos().x();
        int mouseY = event->pos().y();
        if(topLeftDrawingPosition.x()     < mouseX &&
           bottomRightDrawingPosition.x() > mouseX &&
           topLeftDrawingPosition.y()     < mouseY &&
           bottomRightDrawingPosition.y() > mouseY)
        {
            update();
            int cellWidth = (bottomRightDrawingPosition.x() - topLeftDrawingPosition.x()) / grid.getWidth();
            int leftPart = grid.getWidth() * (mouseX - topLeftDrawingPosition.x()) /
                           (bottomRightDrawingPosition.x() - topLeftDrawingPosition.x());
            int rightPart = grid.getWidth() - leftPart;
            int topPart = grid.getWidth() * (mouseY - topLeftDrawingPosition.y()) /
                          (bottomRightDrawingPosition.y() - topLeftDrawingPosition.y());
            int bottomPart = grid.getWidth() - topPart;

            if (event->delta() > 0)
            {
                if (event->orientation() == Qt::Vertical)
                {
                    // if the scrolling has any effect
                    if(floor(cellWidth * mouseScrollSensitivity) > 1 &&
                       floor(cellWidth * mouseScrollSensitivity) != cellWidth)
                    {
                        cellWidth *= mouseScrollSensitivity;
                    }
                    else
                    {
                        cellWidth += 1;
                    }

                }
            }
            else
            {
                if (event->delta() < 0)
                {
                    if (event->orientation() == Qt::Vertical)
                    {
                        // cellWidth should not be less than one pixel
                        if(floor(cellWidth / mouseScrollSensitivity) > 0)
                        {
                            cellWidth /= mouseScrollSensitivity;
                        }
                    }
                }
            }
            topLeftDrawingPosition.setX(event->pos().x() - leftPart * cellWidth);
            topLeftDrawingPosition.setY(event->pos().y() - topPart * cellWidth);
            bottomRightDrawingPosition.setX(event->pos().x() + rightPart * cellWidth);
            bottomRightDrawingPosition.setY(event->pos().y() + bottomPart * cellWidth);
        }
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
    /*case DRAWING:
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
    */
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
            topLeftDrawingPosition.setX(topLeftDrawingPosition.x() + event->pos().x() - mousePosition.x());
            topLeftDrawingPosition.setY(topLeftDrawingPosition.y() + event->pos().y() - mousePosition.y());

            bottomRightDrawingPosition.setX(bottomRightDrawingPosition.x() + event->pos().x() - mousePosition.x());
            bottomRightDrawingPosition.setY(bottomRightDrawingPosition.y() + event->pos().y() - mousePosition.y());
        }
    break;
        /*
    case DRAWING:
        // it is easier to draw only on clicks
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
    */
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
        topLeftDrawingPosition.setX(topLeftDrawingPosition.x() + event->pos().x() - mousePosition.x());
        topLeftDrawingPosition.setY(topLeftDrawingPosition.y() + event->pos().y() - mousePosition.y());

        bottomRightDrawingPosition.setX(bottomRightDrawingPosition.x() + event->pos().x() - mousePosition.x());
        bottomRightDrawingPosition.setY(bottomRightDrawingPosition.y() + event->pos().y() - mousePosition.y());
        update();
    }
}
