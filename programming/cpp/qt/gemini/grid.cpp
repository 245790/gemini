#include "grid.h"

int Grid::findNeighbours(int x, int y)
{
   int neighbours = 0;
   for(int i = -1; i <= 1; i++)
   {
      for(int j = -1; j <= 1; j++)
      {
         if(i == 0 && j == 0)
         {
            continue;
         }
         if(grd[(x + i + height) % height][(y + j + width) % width].getStatus() == ALIVE ||
            grd[(x + i + height) % height][(y + j + width) % width].getStatus() == WAS_ALIVE)
         {
            neighbours++;
         }
      }
   }
   return neighbours;
}

Grid::Grid()
{
    initEmptyGrid(80, 25);
}

void Grid::initEmptyGrid(int width, int height)
{
   this->height = height;
   this->width = width;

   grd.resize(height);
   for(int i = 0; i < height; i++)
   {
      grd[i].resize(width);
   }

   for(int i = 0; i < height; i++)
   {
      for(int j = 0; j < width; j++)
      {
         grd[i][j].setStatus(DEAD);
      }
   }
}

void Grid::initGridFromArray(const QVector<QVector<int> > &initialArray)
{
    initEmptyGrid(initialArray.size(), initialArray[0].size());

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(initialArray[i][j] == 0)
            {
                grd[i][j].setStatus(DEAD);
            }
            else
            {
                grd[i][j].setStatus(ALIVE);
            }
        }
    }
}

void Grid::initRandom(int width, int height)
{
   initEmptyGrid(width, height);
   for(int i = 0; i < height; i++)
   {
      for(int j = 0; j < width; j++)
      {
         if(qrand() % 2 == 0)
         {
            grd[i][j].setStatus(ALIVE);
         }
      }
   }
}

bool Grid::isAlive(int heightIndex, int widthIndex)
{
    return grd[heightIndex][widthIndex].getStatus() == ALIVE;
}

void Grid::setAlive(int heightIndex, int widthIndex, bool isAlive)
{
    grd[heightIndex][widthIndex].setStatus(isAlive ? ALIVE : DEAD);
}

int Grid::getWidth()
{
   return width;
}

int Grid::getHeight()
{
   return height;
}

void Grid::update()
{
   for(int i = 0; i < height; i++)
   {
      for(int j = 0; j < width; j++)
      {
         if(grd[i][j].getStatus() == DEAD &&
            findNeighbours(i, j) == 3)
         {
            grd[i][j].setStatus(WAS_DEAD);
         }
         if(grd[i][j].getStatus() == ALIVE &&
           (findNeighbours(i, j) < 2 || findNeighbours(i, j) > 3))
         {
            grd[i][j].setStatus(WAS_ALIVE);
         }
      }
   }
   for(int i = 0; i < height; i++)
   {
      for(int j = 0; j < width; j++)
      {
         if(grd[i][j].getStatus() == ALIVE)
         {
            grd[i][j].incrementAge();
         }
         if(grd[i][j].getStatus() == WAS_DEAD)
         {
            grd[i][j].setStatus(ALIVE);
            grd[i][j].nullifyAge();
         }

         if(grd[i][j].getStatus() == WAS_ALIVE)
         {
            grd[i][j].setStatus(DEAD);
            grd[i][j].nullifyAge();
         }
      }
   }
}
