#include <iostream>
#include <math.h>
#include <qvector.h>

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
    while(maxDimension > pow(2, i));
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

bool Grid::isAlive(int heightIndex, int widthIndex)
{
    return root->getBit(widthIndex, heightIndex) == 1 ? true : false;
}

void Grid::setAlive(int heightIndex, int widthIndex, bool isAlive)
{
    if(widthIndex > pow(2, root->getLevel()) || heightIndex > pow(2, root->getLevel()))
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
