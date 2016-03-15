/* KPCC
 * Class Grid is an interface for celluar automaton
 * Author: Safin Karim
 * Date: 2014.12.4
 */

#include <fstream>
#include <math.h>
#include <QDebug>
#include <QFile>

#include "grid.h"

Grid::Grid()
{
    initEmptyGrid(80, 25);
}

void Grid::initEmptyGrid(int width, int height)
{
    int maxDimension = width > height ? width : height;
    int i = 0; // minimum power of 2 that exceeds maxDimension
    do
    {
        i++;
    }
    while(maxDimension > 1 << i);
    root = root->emptyTree(i);
    generationCount = 0;
}

void Grid::initRandom(int width, int height)
{
    for(int i = - height / 2; i < height / 2; i++)
    {
        for(int j = - width / 2; j < width / 2; j++)
        {
            if(qrand() % 2 == 0)
            {
                this->setAlive(i, j, true);
            }
        }
    }
    generationCount = 0;
}

void Grid::parsePlainText(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream fin(&file);
    this->clear();
    QString currentString;
    do
    {
        currentString = fin.readLine();
    }
    while(currentString[0] == '!');

    QString lineOfBody = currentString; // that is, the first row of cells

    QVector<QString> body;

    body.push_back(lineOfBody);
    int maxWidth = lineOfBody.length(); // width of a pattern

    while(!fin.atEnd())
    {
        lineOfBody = fin.readLine();
        body.push_back(lineOfBody);
        if(maxWidth < lineOfBody.length())
        {
            maxWidth = lineOfBody.length();
        }
    }

    for(int i = 0; i < body.size(); i++)
    {
        for(int j = 0; j < body[i].length(); j++)
        {
            if(body[i][j] != '.')
            {
                this->setAlive(i - body.size() / 2, j - maxWidth / 2, true);
            }
        }
    }
    file.close();
}

void Grid::parseRLE(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream fin(&file);
    this->clear();
    QString inputLine;
    int x = 0, y = 0;      // current location
    int paramArgument = 0; // our parameter location
    while(!fin.atEnd())
    {
        inputLine = fin.readLine();
        if(inputLine.length() != 0 && (inputLine[0] == 'x' || inputLine[0] == '#'))
        {
            continue; // We do not care of comment lines
        }
        for(int i = 0; i < inputLine.length(); i++)
        {
            QChar c = inputLine[i];
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
                        this->setAlive(x++, y, true);
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
                            paramArgument = 10 * paramArgument + c.digitValue();
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
    // move the pattern to the center
    root = root.get()->getse();
    file.close();
}

void Grid::clear()
{
    root = root->emptyTree(root->getLevel());
    generationCount = 0;
}

bool Grid::isAlive(int heightIndex, int widthIndex)
{
    return root->getBit(widthIndex, heightIndex) == 1 ? true : false;
}

void Grid::setAlive(int heightIndex, int widthIndex, bool isAlive)
{
    // If an index does not fit into grid
    while(abs(widthIndex) > getWidth() / 2 || abs(heightIndex) > getHeight() / 2)
    {
        root = root->expandUniverse();
    }
    if(isAlive)
    {
        root = root->setBit(widthIndex, heightIndex);
    }
    else
    {
        root = root->unsetBit(widthIndex, heightIndex);
    }
}

int Grid::getWidth()
{
    return 1 << root->getLevel();
}

int Grid::getHeight()
{
    return 1 << root->getLevel();
}

/**
*   Run a step.  First, we make sure the root is large enough to
*   include the entire next generation by checking that all border
*   nodes in the 4x4 square three levels down are empty.  Then we
*   simply invoke the next generation method of the node.
*/
void Grid::update()
{
    while(root->getLevel() < 3 ||
          root->getnw()->getPopulation() != root->getnw()->getse()->getse()->getPopulation() ||
          root->getne()->getPopulation() != root->getne()->getsw()->getsw()->getPopulation() ||
          root->getsw()->getPopulation() != root->getsw()->getne()->getne()->getPopulation() ||
          root->getse()->getPopulation() != root->getse()->getnw()->getnw()->getPopulation())
    {
        root = root->expandUniverse();
    }
    root = root->nextGeneration();
    generationCount++;
}

void Grid::draw(QPainter* painter, int x0, int y0, float width)
{
    root->recDraw(painter, x0, y0, width);
}

int Grid::getGeneration()
{
    return generationCount;
}

long Grid::getPopulation()
{
    return root->getPopulation();
}

void Grid::rotateClockwise()
{
    root = root->rotateClockwise();
}

void Grid::rotateAntiClockwise()
{
    root = root->rotateAntiClockwise();
}
