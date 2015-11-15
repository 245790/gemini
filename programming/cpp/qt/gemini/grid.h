#ifndef GRID_H
#define GRID_H

#include <QVector>
#include <QPainter>

#include "cell.h"
#include "treenode.h"

using namespace std;

class Grid
{
private:
    int generationCount;
    TreeNode* root;
public:
    Grid();
    void initEmptyGrid(int width, int height);
    void initRandom(int width, int height);
    bool isAlive(int heightIndex, int widthIndex);
    void setAlive(int heightIndex, int widthIndex, bool isAlive);
    int getWidth();
    int getHeight();
    void update();
    void draw(QPainter* painter, int x0, int y0, float width);
};

#endif // GRID_H

