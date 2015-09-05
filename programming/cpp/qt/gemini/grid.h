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
    void initGridFromArray(QVector<QVector<int> > initialArray);
    void initRandom(int width, int height);
    int getWidth();
    int getHeight();
    void update();
};

#endif // GRID_H

