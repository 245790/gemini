/* KPCC
 * GridPainter is a widget able to draw Grids
 * File: gridpainter.cpp
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

    grid.initEmptyGrid(1000, 1000);

    topLeftDrawingPosition = QPoint(-grid.getWidth() * 5,
                                    -grid.getWidth() * 5);

    // taking grid.getWidth() * 5 ensures that cell size will be more than 1
    // if the size of a cell is less than one, the program won't work correctly
    bottomRightDrawingPosition = QPoint(grid.getWidth() * 5,
                                        grid.getWidth() * 5);

    cellColor = QColor(0, 0, 0);
    spaceColor = QColor(255, 255, 255);
    gridColor = QColor(230, 230, 230);

    cellBrush.setColor(cellColor);
    cellBrush.setStyle(Qt::SolidPattern);

    gridPen.setColor(gridColor);
    gridPen.setStyle(Qt::SolidLine);

    mouseScrollSensitivity = 1.3;

    mousePosition = QPoint(0, 0);
    setMouseMode(MOVING);

    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_Hover);

    currentPaintingIndex = 0;
    currentErasingIndex = 0;
}

void GridPainter::autoFitDrawingPoints()
{
    int left = grid.leftBoundary();
    int right = grid.rightBoundary();
    int bottom = grid.bottomBoundary();
    int top = grid.topBoundary();
    // cellWidth is maximal possible width of a cell so that all the cells fit
    // into the screen
    int cellWidth;
    int width = right - left;
    int height = bottom - top;
    if (this->width() / this->height() > width / height)
    {
        if (width < this->width())
        {
            cellWidth = this->height() / height;
        }
        else
        {
            cellWidth = 1;
        }
    }
    else
    {
        if (height < this->height())
        {
            cellWidth = this->width() / width;
        }
        else
        {
            cellWidth = 1;
        }
    }
    topLeftDrawingPosition = QPoint(-left * cellWidth, -top * cellWidth);
    int leftCoord = topLeftDrawingPosition.x();
    int topCoord = topLeftDrawingPosition.y();
    bottomRightDrawingPosition = QPoint(leftCoord + grid.getWidth() * cellWidth,
                                        topCoord + grid.getWidth() * cellWidth);
}

void GridPainter::preventResizing(int prevGridWidth,
                                  int currentGridWidth,
                                  int prevFieldWidth)
{
    if (prevGridWidth != currentGridWidth) // prevent unneeded resizing
    {
        int prevWidth = bottomRightDrawingPosition .x() -
                             topLeftDrawingPosition.x();
        if (prevGridWidth < currentGridWidth)
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
    if (!stopped)
    {
        int prevGridWidth = grid.getWidth();
        grid.update();
        int currentGridWidth = grid.getWidth();
        preventResizing(prevGridWidth,
                        currentGridWidth,
                        bottomRightDrawingPosition.x() -
                            topLeftDrawingPosition.x());

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
    if (stopped)
    {
        int prevGridWidth = grid.getWidth();
        grid.update();
        int currentGridWidth = grid.getWidth();
        preventResizing(prevGridWidth,
                        currentGridWidth,
                        bottomRightDrawingPosition.x() -
                            topLeftDrawingPosition.x());
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

bool GridPainter::parsePlainText(const QString &fileName)
{
    stopped = true;
    bool success = grid.parsePlainText(fileName);
    autoFitDrawingPoints();
    return success;
}

bool GridPainter::parseRLE(const QString &fileName)
{
    stopped = true;
    bool success = grid.parseRLE(fileName);
    autoFitDrawingPoints();
    return success;
}

void GridPainter::saveAsPlainText(const QString &fileName)
{
    grid.saveAsPlainText(fileName);
}

void GridPainter::saveAsRLE(const QString &fileName)
{
    grid.saveAsRLE(fileName);
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

bool GridPainter::setDrawingPattern(int index, const QString& fileName)
{
    if (index > 10 || index < 0) // we do not store more than 10
    {
        return false;
    }
    if (index >= painting.size())
    {
        painting.resize(index + 1);
    }
    return painting[index].parseRLE(fileName);
}

bool GridPainter::setErasingPattern(int index, const QString& fileName)
{
    if (index > 10 || index < 0) // we do not store more than 10
    {
        return false;
    }
    if (index >= erasing.size())
    {
        erasing.resize(index + 1);
    }
    return erasing[index].parseRLE(fileName);
}

void GridPainter::setCurrentPattern(int index)
{
    if (mode == DRAWING)
    {
        setCurrentDrawingPattern(index);
    }
    if (mode == ERASING)
    {
        setCurrentErasingPattern(index);
    }
    update();
}

void GridPainter::setCurrentDrawingPattern(int index)
{
    if (painting.size() > index)
    {
        currentPaintingIndex = index;
    }
}

void GridPainter::setCurrentErasingPattern(int index)
{
    if (erasing.size() > index)
    {
        currentErasingIndex = index;
    }
}

int GridPainter::getHashSize()
{
    return grid.hashSize();
}

void GridPainter::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter;
    painter->begin(this);

    painter->fillRect(event->rect(), spaceColor);

    painter->setBrush(cellBrush);

    int fieldWidth = (bottomRightDrawingPosition.x() -
                          topLeftDrawingPosition.x());

    painter->save();
    painter->setPen(Qt::NoPen);
    grid.draw(painter,
              topLeftDrawingPosition.x() + fieldWidth / 2,
              topLeftDrawingPosition.y() + fieldWidth / 2,
              fieldWidth);

    int mouseX = mousePosition.x() - topLeftDrawingPosition.x();
    int mouseY = mousePosition.y() - topLeftDrawingPosition.y();

    int cellWidth = fieldWidth / grid.getWidth();

    mouseX /= cellWidth;
    mouseY /= cellWidth;

    mouseX = topLeftDrawingPosition.x() + mouseX * cellWidth;
    mouseY = topLeftDrawingPosition.y() + mouseY * cellWidth;

    // if the mouse is inside the field
    if (mouseX >     topLeftDrawingPosition.x() &&
        mouseY >     topLeftDrawingPosition.y() &&
        mouseX < bottomRightDrawingPosition.x() &&
        mouseY < bottomRightDrawingPosition.y())
    {
        if (mode == DRAWING)
        {
            // draw the pattern that is about to be inserted
            painter->setBrush(cellBrush);
            float paintingWidth = painting[currentPaintingIndex].getWidth()
                                      * cellWidth;
            painting[currentPaintingIndex].draw(painter,
                                                mouseX,
                                                mouseY,
                                                paintingWidth);
        }

        if (mode == ERASING)
        {
            // draw the pattern that is about to be erased with
            painter->setBrush(QBrush(gridColor));
            float erasingWidth = erasing[currentErasingIndex].getWidth()
                                      * cellWidth;
            erasing[currentErasingIndex].draw(painter,
                                                mouseX,
                                                mouseY,
                                                erasingWidth);
        }
    }

    if (cellWidth > 3) // if a cell is big enough, then draw the grid
    {
        painter->setPen(gridPen);
        for (int i = topLeftDrawingPosition.x() % cellWidth;
                i < event->rect().width();
                i += cellWidth)
        {
            painter->drawLine(i,
                              0,
                              i,
                              event->rect().height());
        }
        for (int i = topLeftDrawingPosition.y() % cellWidth;
                i < event->rect().height();
                i += cellWidth)
        {
            painter->drawLine(0,
                              i,
                              event->rect().width(),
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
    {
        int mouseX = event->pos().x();
        int mouseY = event->pos().y();
        if (topLeftDrawingPosition.x()    < mouseX &&
           bottomRightDrawingPosition.x() > mouseX &&
           topLeftDrawingPosition.y()     < mouseY &&
           bottomRightDrawingPosition.y() > mouseY)
        {
            update();
            int cellWidth = (bottomRightDrawingPosition.x() -
                                 topLeftDrawingPosition.x()) / grid.getWidth();
            int leftPart = grid.getWidth() * (mouseX -
                                              topLeftDrawingPosition.x()) /
                           (bottomRightDrawingPosition.x() -
                                topLeftDrawingPosition.x());
            int rightPart = grid.getWidth() - leftPart;
            int topPart = grid.getWidth() * (mouseY -
                                             topLeftDrawingPosition.y()) /
                          (bottomRightDrawingPosition.y() -
                               topLeftDrawingPosition.y());
            int bottomPart = grid.getWidth() - topPart;

            if (event->delta() > 0)
            {
                if (event->orientation() == Qt::Vertical)
                {
                    // if the scrolling has any effect
                    if (floor(cellWidth * mouseScrollSensitivity) > 1 &&
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
                        if (floor(cellWidth / mouseScrollSensitivity) > 0)
                        {
                            cellWidth /= mouseScrollSensitivity;
                        }
                    }
                }
            }
            topLeftDrawingPosition.setX(event->pos().x() -
                                        leftPart * cellWidth);
            topLeftDrawingPosition.setY(event->pos().y() -
                                        topPart * cellWidth);
            bottomRightDrawingPosition.setX(event->pos().x() +
                                            rightPart * cellWidth);
            bottomRightDrawingPosition.setY(event->pos().y() +
                                            bottomPart * cellWidth);
        }
    }
    break;
    case DRAWING:
        if (event->delta() > 0)
        {
            if (event->orientation() == Qt::Vertical)
            {
                painting[currentPaintingIndex].rotateAntiClockwise();
            }
        }
        else
        {
            if (event->delta() < 0)
            {
                if (event->orientation() == Qt::Vertical)
                {
                    painting[currentPaintingIndex].rotateClockwise();
                }
            }
        }
    break;
    case ERASING:
        if (event->delta() > 0)
        {
            if (event->orientation() == Qt::Vertical)
            {
                erasing[currentErasingIndex].rotateAntiClockwise();
            }
        }
        else
        {
            if (event->delta() < 0)
            {
                if (event->orientation() == Qt::Vertical)
                {
                    erasing[currentErasingIndex].rotateClockwise();
                }
            }
        }
    break;
    }
    update();
}
#endif

void GridPainter::mousePressEvent(QMouseEvent *event)
{
    int mouseX = mousePosition.x();
    int mouseY = mousePosition.y();
    // if the mouse is inside the field
    if (mouseX >     topLeftDrawingPosition.x() &&
        mouseY >     topLeftDrawingPosition.y() &&
        mouseX < bottomRightDrawingPosition.x() &&
        mouseY < bottomRightDrawingPosition.y())
    {
        int fieldWidth = (bottomRightDrawingPosition.x() -
                              topLeftDrawingPosition.x());

        mouseX -= topLeftDrawingPosition.x();
        mouseY -= topLeftDrawingPosition.y();

        int cellWidth = fieldWidth / grid.getWidth();

        mouseX /= cellWidth;
        mouseY /= cellWidth;

        mouseX -= grid.getWidth() / 2;
        mouseY -= grid.getWidth() / 2;

        switch(mode)
        {
        case MOVING:
        break;
        case DRAWING:
        {
            int oldGridWidth = grid.getWidth();
            grid.insertPattern(painting[currentPaintingIndex],
                               mouseY, // not a bug
                               mouseX, // y, then x
                               true);  // we are painting, therefore the new
                                       // cells will be alive
            if (oldGridWidth != grid.getWidth())
            {
                preventResizing(oldGridWidth,
                                grid.getWidth(),
                                bottomRightDrawingPosition.x() -
                                    topLeftDrawingPosition.x());
            }
        }
        break;
        case ERASING:
            int oldGridWidth = grid.getWidth();
            grid.insertPattern(erasing[currentErasingIndex],
                               mouseY, // not a bug
                               mouseX, // y, then x
                               false); // we are erasing, therefore the new
                                       // cells will be dead
            if (oldGridWidth != grid.getWidth())
            {
                preventResizing(oldGridWidth,
                                grid.getWidth(),
                                bottomRightDrawingPosition.x() -
                                    topLeftDrawingPosition.x());
            }
        break;
        }
    }
    update();
}

void GridPainter::mouseMoveEvent(QMouseEvent *event)
{
    switch(mode)
    {
    case MOVING:
        if ((event->buttons() & Qt::LeftButton))
        {
            topLeftDrawingPosition.setX(topLeftDrawingPosition.x() +
                                            event->pos().x() -
                                            mousePosition.x());
            topLeftDrawingPosition.setY(topLeftDrawingPosition.y() +
                                            event->pos().y() -
                                            mousePosition.y());

            bottomRightDrawingPosition.setX(bottomRightDrawingPosition.x() +
                                            event->pos().x() -
                                            mousePosition.x());
            bottomRightDrawingPosition.setY(bottomRightDrawingPosition.y() +
                                            event->pos().y() -
                                            mousePosition.y());
        }
    case DRAWING:
    case ERASING:
    break;
    }
    update();
}

void GridPainter::hoverMove(QHoverEvent *event)
{
    mousePosition.setX(event->pos().x());
    mousePosition.setY(event->pos().y());
    update();

}

bool GridPainter::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::HoverMove:
        hoverMove(static_cast<QHoverEvent*>(event));
        return true;
    break;
    default:
    break;
    }
    return QWidget::event(event);
}
