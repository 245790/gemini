/* KPCC
 * Class Grid is an interface for celluar automaton
 * Author: Safin Karim
 * Date: 2014.12.4
 */

#include <fstream>
#include <limits>
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
    int i = ceil(log2(maxDimension));
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

bool Grid::parsePlainText(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream fin(&file);
    this->clear();

    bool success = true;

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
        if (maxWidth > 3000) // too big pattern; probably an error
        {
            success = false;
            break;
        }
        if (body.length() > 3000) // too big pattern; probably an error
        {
            success = false;
            break;
        }
    }

    file.close();

    if (!success)
    {
        return false;
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

    return success; // that is, true
}

bool Grid::parseRLE(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream fin(&file);
    this->clear();
    this->initEmptyGrid(5, 5);
    QString inputLine;
    int x = 0, y = 0;      // current location
    int paramArgument = 0; // our parameter location

    bool success = true;
    while(!fin.atEnd())
    {
        inputLine = fin.readLine();
        if(inputLine.length() != 0 && (inputLine[0] == 'x'
                                       || inputLine[0] == '#'))
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
                                success = true;
                                break;
                            }
                            else
                            {
                                if(c == ' ')
                                {
                                    continue;
                                }
                                else
                                {
                                    success = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // move the pattern to the centre
    if (!root->getnw()->isAlive() &&
        !root->getne()->isAlive() &&
        !root->getsw()->isAlive() &&
        success)
    {
        root = root->getse();
    }
    file.close();
    return success;
}

void Grid::saveAsPlainText(const QString &fileName)
{
    QVector<QVector<int> > cells = as2dArray();
    if (cells.size() == 0)
    {
        return;
    }
    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Truncate | QFile::Text))
    {
        return;
    }
    QTextStream stream(&file);
    stream << "!Created in Gemini\n";
    for (int i = 0; i < cells.size(); ++i)
    {
        for (int j = 0; j < cells[i].size(); ++j)
        {
            if (cells[i][j] == 0)
            {
                stream << ".";
            }
            else
            {
                stream << "O";
            }
        }
        stream << "\n";
    }
}

void Grid::saveAsRLE(const QString &fileName)
{
    QVector<QVector<int> > cells = as2dArray();
    if (cells.size() == 0)
    {
        return;
    }
    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Truncate | QFile::Text))
    {
        return;
    }
    QTextStream stream(&file);
    stream << "#C Created in Gemini\n";
    int width = cells[0].size();
    int height = cells.size();
    stream << "x = " << width << ",y = " << height
           << "rule = b3/s23\n";
    int run = 1; // number of identical cells going successively
    for (int i = 0; i < width; ++i)
    {
        run = 1;
        for (int j = 0; j < height - 1; ++j)
        {
            if (cells[j][i] == cells[j + 1][i])
            {
                ++run;
            }
            else
            {
                if (run != 1)
                {
                    stream << run;
                }
                if (cells[j][i] == 1)
                {
                    stream << "o";
                }
                else
                {
                    stream << "b";
                }
                run = 1;
            }
        }
        if (cells[height - 1][i] != 0)
        {
            if (run != 1)
            {
                stream << run;
            }
            stream << "o";
            // if dead cells are trailing at the end, we don't need to write
        }
        stream << "$";
    }
    stream << "!";
}

void Grid::clear()
{
    root = root->emptyTree(root->getLevel());
    generationCount = 0;
}

bool Grid::isEmpty() const
{
    return root->isAlive();
}

bool Grid::isAlive(int heightIndex, int widthIndex) const
{
    return root->getBit(widthIndex, heightIndex) == 1 ? true : false;
}

void Grid::setAlive(int heightIndex, int widthIndex, bool isAlive)
{
    // If an index does not fit into grid
    while(abs(widthIndex) > getWidth() / 2 || abs(heightIndex) > getHeight() /2)
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

int Grid::getWidth() const
{
    return 1 << root->getLevel();
}

int Grid::getHeight() const
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
          root->getnw()->getPopulation() != root->
                                            getnw()->
                                            getse()->
                                            getse()->
                                            getPopulation() ||
          root->getne()->getPopulation() != root->
                                            getne()->
                                            getsw()->
                                            getsw()->
                                            getPopulation() ||
          root->getsw()->getPopulation() != root->
                                            getsw()->
                                            getne()->
                                            getne()->
                                            getPopulation() ||
          root->getse()->getPopulation() != root->
                                            getse()->
                                            getnw()->
                                            getnw()->
                                            getPopulation())
    {
        root = root->expandUniverse();
    }
    root = root->nextGeneration();
    generationCount++;
}

void Grid::draw(QPainter* painter, int x0, int y0, float width) const
{
    root->recDraw(painter, x0, y0, width);
}

int Grid::getGeneration() const
{
    return generationCount;
}

long Grid::getPopulation() const
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

int Grid::leftBoundary() const
{
    int lb = root->leftBoundary();
    return lb != std::numeric_limits<int>::max() ? lb : 0;
}

int Grid::rightBoundary() const
{
    int rb = root->rightBoundary();
    return rb != std::numeric_limits<int>::min() ? rb : getWidth() - 1;
}

int Grid::bottomBoundary() const
{
    int bb = root->bottomBoundary();
    return bb != std::numeric_limits<int>::min() ? bb : getWidth() - 1;
}

int Grid::topBoundary() const
{
    int tb = root->topBoundary();
    return tb != std::numeric_limits<int>::max() ? tb : 0;
}

void Grid::insertPattern(const Grid& pattern, int xPos, int yPos, bool alive)
{
    for (int i = -pattern.getWidth() / 2;
         i < pattern.getWidth() / 2;
         i++)
    {
        for (int j = -pattern.getWidth() / 2;
             j < pattern.getWidth() / 2;
             j++)
        {
            if (pattern.isAlive(i, j))
            {
                this->setAlive(i + xPos, j + yPos, alive);
            }
        }
    }
}

QVector<QVector<int> > Grid::as2dArray() const
{
    QVector<QVector<int> > result;
    if (!root->isAlive())
    {
        return result;
    }

    int left = leftBoundary();
    int right = rightBoundary();
    int top = topBoundary();
    int bottom = bottomBoundary();
    int height = bottom - top + 1;
    int width = right - left + 1;
    int gridWidth = getWidth();
    result.resize(height);
    for (int i = 0; i < result.size(); ++i)
    {
        result[i].resize(width);
    }
    for (int i = top; i <= bottom; ++i)
    {
        for (int j = left; j <= right; ++j)
        {
            result[i - top][j - left] = root->getBit(j - gridWidth / 2,
                                                     i - gridWidth / 2);
        }
    }
    return result;
}

int Grid::hashSize()
{
    return TreeNode::hashSize();
}
