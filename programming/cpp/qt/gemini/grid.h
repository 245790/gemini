#ifndef GRID_H
#define GRID_H

#include <QPainter>
#include <QString>

#include "treenode.h"

using namespace std;

class Grid
{
private:
    int generationCount; // number of a generation passed since creation
    TreeNode* root; // actually a grid
public:
    Grid();
    void initEmptyGrid(int width, int height);
    void initRandom(int width, int height);
    void parsePlainText(const QString &fileName);
    void parseRLE(const QString &fileName);
    void clear();
    bool isAlive(int heightIndex, int widthIndex); // true if root[hI][wI] is alive
    void setAlive(int heightIndex, int widthIndex, bool isAlive);
    int getWidth();
    int getHeight(); // returns the same as getWidth
    void update(); // calculates next generation and sometimes expands this
    // draws itself so that (x0, y0) is in the center
    void draw(QPainter* painter, int x0, int y0, float width);
    int getGeneration(); // returns generationCount
    void rotateClockwise();
    void rotateAntiClockwise();
};

#endif // GRID_H

