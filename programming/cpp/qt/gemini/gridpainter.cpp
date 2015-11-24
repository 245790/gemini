#include <fstream>
#include <string>
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
    
    cellBrush.setColor(QColor(0, 0, 0));
    cellBrush.setStyle(Qt::SolidPattern);

    spaceBrush.setColor(QColor(250, 250, 250));
    spaceBrush.setStyle(Qt::SolidPattern);

    mode = MOVING;

    setFocusPolicy(Qt::ClickFocus);
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
    std::ifstream fin;
    fin.open(fileName.toStdString().c_str());
    grid.clear();
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
    drawingPosition.setX(4 * scale * grid.getWidth() / 2);
    drawingPosition.setY(4 * scale * grid.getHeight() / 2);
    fin.close();
}

void GridPainter::parseRLE(const QString &fileName)
{
    stopped = true;
    std::ifstream fin;
    grid.clear();
    fin.open(fileName.toStdString().c_str());
    if(fin.good())
    {
        string inputLine;
        int x = 0, y = 0;      // current location
        int paramArgument = 0; // our parameter location
        while(!fin.eof())
        {
            std::getline(fin, inputLine);
            if(inputLine.length() != 0 && (inputLine[0] == 'x' || inputLine[0] == '#'))
            {
                continue; // We do not care of comment lines
            }
            for(unsigned int i = 0; i < inputLine.length(); i++)
            {
                char c = inputLine[i];
                int param = (paramArgument == 0 ? 1 : paramArgument);
                if(c == 'b')
                {
                    x += param;
                    paramArgument = 0;
                }
                else
                {
                    if(c == 'o')
                    {
                        while(param-- > 0)
                        {
                            grid.setAlive(x++, y, true);
                        }
                        paramArgument = 0;
                    }
                    else
                    {
                        if(c == '$')
                        {
                            y += param;
                            x = 0;
                            paramArgument = 0;
                        }
                        else
                        {
                            if('0' <= c && c <= '9')
                            {
                                paramArgument = 10 * paramArgument + c - '0';
                            }
                            else
                            {
                                if(c == '!')
                                {
                                    return;
                                }
                                else
                                {
                                    if(c == ' ')
                                    {
                                        continue;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    drawingPosition.setX(4 * scale * grid.getWidth() / 2);
    drawingPosition.setY(4 * scale * grid.getHeight() / 2);
    fin.close();
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
//    painter->fillRect(this->mapFromGlobal(QCursor::pos()).x() - this->mapFromGlobal(QCursor::pos()).x() % cellWidth - 1,
//                      this->mapFromGlobal(QCursor::pos()).y() - this->mapFromGlobal(QCursor::pos()).y() % cellWidth - 1,
//                      cellWidth,
//                      cellWidth,
//                      Qt::red);
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
        update();
        if (event->delta() > 0)
        {

            if (event->orientation() == Qt::Vertical)
            {
                scale *= 1.1;
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
                        scale /= 1.1;
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
            mousePosition.setX(event->pos().x());
            mousePosition.setY(event->pos().y());
            mousePressed = true;
            update();
        }
    break;
    case DRAWING:
        if (event->button() == Qt::LeftButton)
        {
            update();
            // 2 is a magic number caused by some Qt errors
            int xPosition = (event->pos().x() - drawingPosition.x()) / cellWidth - 2;
            int yPosition = (event->pos().y() - drawingPosition.y()) / cellWidth - 2;
            // if the mouse is pressed, make the cell alive
            grid.setAlive(yPosition,
                          xPosition,
                          true);
        }
    break;
    }
}

void GridPainter::mouseMoveEvent(QMouseEvent *event)
{
    update();
    switch(mode)
    {
    case MOVING:
        if ((event->buttons() & Qt::LeftButton))
        {
            drawingPosition.setX(drawingPosition.x() + event->pos().x() - mousePosition.x());
            drawingPosition.setY(drawingPosition.y() + event->pos().y() - mousePosition.y());
            mousePosition.setX(event->pos().x());
            mousePosition.setY(event->pos().y());
            update();
        }
    break;
    case DRAWING:

        int xPosition = (event->pos().x() - drawingPosition.x()) / cellWidth - 2;
        int yPosition = (event->pos().y() - drawingPosition.y()) / cellWidth - 2;
        // if the mouse is pressed, then actually make the cell alive
        if ((event->buttons() & Qt::LeftButton))
        {
            grid.setAlive(yPosition,
                          xPosition,
                          true);
        }
        update();
    break;
    }
}

void GridPainter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        drawingPosition.setX(drawingPosition.x() + event->pos().x() - mousePosition.x());
        drawingPosition.setY(drawingPosition.y() + event->pos().y() - mousePosition.y());
        mousePosition.setX(event->pos().x());
        mousePosition.setY(event->pos().y());
        update();
    }
}

void GridPainter::keyPressEvent(QKeyEvent * event)
{
    switch(event->key())
    {
    case Qt::Key_M:
        mode = MOVING;
        break;
    case Qt::Key_D:
        mode = DRAWING;
        break;
    }
    update();
}
