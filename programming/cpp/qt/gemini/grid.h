#ifndef GRID_H
#define GRID_H

#include <QVector>

#include "cell.h"

using namespace std;

class Grid
{
private:
    QVector<QVector<Cell> > grd;
    int width;
    int height;
    int findNeighbours(int x, int y);
public:
    Grid();
    void initEmptyGrid(int width, int height);
    void initGridFromArray(const QVector<QVector<int> > &initialArray);
    void initRandom(int width, int height);
    bool isAlive(int heightIndex, int widthIndex);
    void setAlive(int heightIndex, int widthIndex, bool isAlive);
    int getWidth();
    int getHeight();
    void update();
};

#endif // GRID_H

