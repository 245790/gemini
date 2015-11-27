#include <math.h>

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
    //initEmptyGrid(width, height);
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(qrand() % 2 == 0)
            {
                while(i >= getWidth() / 2 || j >= getWidth() / 2)
                {
                    root = root->expandUniverse();
                }
                root = root->setBit(i, j);
            }
        }
    }
    generationCount = 0;
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
    while(widthIndex > 1 << root->getLevel() || heightIndex > 1 << root->getLevel())
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

void Grid::rotateClockwise()
{
    root = root->rotateClockwise();
}

void Grid::rotateAntiClockwise()
{
    root = root->rotateAntiClockwise();
}
