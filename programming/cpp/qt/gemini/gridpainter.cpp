#include <fstream>
#include <string>
#include <QPainter>
#include <QPaintEvent>
#include <QtWidgets>
#include <QWidget>

#include "gridpainter.h"

GridPainter::GridPainter(QWidget *parent) : QOpenGLWidget(parent)
{
    stopped = true;

    mousePressed = false;

    drawingPosition = QPoint(0, 0);

    cellColor = QColor(0, 0, 0);
    spaceColor = QColor(240, 240, 240);

    cellPen = QPen(cellColor);
    cellPen.setStyle(Qt::NoPen);
    
    cellBrush.setColor(QColor(0, 0, 0));
    cellBrush.setStyle(Qt::SolidPattern);

    spaceBrush.setColor(QColor(250, 250, 250));
    spaceBrush.setStyle(Qt::SolidPattern);

    scale = 1;

    mode = MOVING;
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

void GridPainter::initEmptyGrid(int width, int height)
{
    grid.initEmptyGrid(width, height);
}

void GridPainter::parsePlainText(const QString &fileName)
{
    std::ifstream fin;
    fin.open(fileName.toStdString().c_str());
    if(fin.good())
    {
        string currentString;
        do
        {
            std::getline(fin, currentString);
        }
        while(currentString[0] == '!');

        string lineOfBody = currentString; //that is, the first row of cells

        QVector<string> body;

        body.push_back(lineOfBody);

        while(!fin.eof())
        {
            getline(fin, lineOfBody);
            body.push_back(lineOfBody);
        }

        int height = body.size();

        unsigned int maxWidth = body[0].length();
        for(int i = 1; i < body.size(); i++)
        {
            if(body[i].length() > maxWidth)
            {
                maxWidth = body[i].length();
            }
        }

        int width = maxWidth;

        grid.initEmptyGrid(width * 2, height * 2);

        for(int i = 0; i < body.size(); i++)
        {
            for(unsigned int j = 0; j < body[i].length(); j++)
            {
                if(body[i][j] != '.')
                {
                    grid.setAlive(i, j, true);
                }
            }
        }
    }
    else
    {
        int height = 25;
        int width = 80;
        grid.initEmptyGrid(width, height);
    }
    drawingPosition.setX(0);
    drawingPosition.setY(0);
    fin.close();
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
        cellWidth = event->rect().height() / grid.getHeight() * scale;
        if(grid.getWidth() * cellWidth > event->rect().width())
        {
            cellWidth = event->rect().width() / grid.getWidth() * scale;
        }
        if(grid.getHeight() * cellWidth > event->rect().height())
        {
            cellWidth = event->rect().height() / grid.getHeight() * scale;
        }
        painter->fillRect(QRectF(drawingPosition.x(),
                                 drawingPosition.y(),
                                 grid.getWidth() * cellWidth,
                                 grid.getHeight() * cellWidth),
                          spaceBrush);
    }
    else
    {
        cellWidth = event->rect().width() / grid.getWidth() * scale;
        if(grid.getWidth() * cellWidth > event->rect().width())
        {
            cellWidth = event->rect().width() / grid.getWidth() * scale;
        }
        if(grid.getHeight() * cellWidth > event->rect().height())
        {
            cellWidth = event->rect().height() / grid.getHeight() * scale;
        }
        painter->fillRect(QRectF(drawingPosition.x() - grid.getWidth() * cellWidth / 2,
                                 drawingPosition.y() - grid.getWidth() * cellWidth / 2,
                                 grid.getWidth() * cellWidth,
                                 grid.getHeight() * cellWidth),
                          spaceBrush);
    }

    painter->save();
    grid.draw(painter, drawingPosition.x(), drawingPosition.y(), grid.getWidth() * cellWidth);
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
        if (event->delta() > 0)
        {

            if (event->orientation() == Qt::Vertical)
            {
                scale *= 1.2;
            }
            update();
        }
        else
        {
            if (event->delta() < 0)
            {
                if (event->orientation() == Qt::Vertical)
                {
                    scale /= 1.2;
                }
            }
            update();
        }
    break;
    case DRAWING:
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
            mousePosition.setX(event->pos().x());
            mousePosition.setY(event->pos().y());
            mousePressed = true;
            update();
        }
    break;
    case DRAWING:
        if (event->button() == Qt::LeftButton)
        {
            int yPosition = float((event->pos().y() - drawingPosition.y()) / this->height()) * grid.getHeight();
            int xPosition = float((event->pos().x() - drawingPosition.x()) / this->width()) * grid.getWidth();
            grid.setAlive(yPosition,
                          xPosition,
                          true);
            update();
        }
    break;
    }
}

void GridPainter::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && mousePressed)
    {
        drawingPosition.setX(drawingPosition.x() + event->pos().x() - mousePosition.x());
        drawingPosition.setY(drawingPosition.y() + event->pos().y() - mousePosition.y());
        mousePosition.setX(event->pos().x());
        mousePosition.setY(event->pos().y());
        update();
    }
}

void GridPainter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && mousePressed)
    {
        drawingPosition.setX(drawingPosition.x() + event->pos().x() - mousePosition.x());
        drawingPosition.setY(drawingPosition.y() + event->pos().y() - mousePosition.y());
        mousePosition.setX(event->pos().x());
        mousePosition.setY(event->pos().y());
        mousePressed = false;
        update();
    }
}
