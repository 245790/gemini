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
    TreeNode* root;
public:
    Grid();
    void initEmptyGrid(int width, int height);
    void initRandom(int width, int height);
    void parsePlainText(const QString &fileName);
    void parseRLE(const QString &fileName);
    void clear();
    bool isAlive(int heightIndex, int widthIndex);
    void setAlive(int heightIndex, int widthIndex, bool isAlive);
    int getWidth();
    int getHeight();
    void update();
    void draw(QPainter* painter, int x0, int y0, float width);
    int getGeneration();
    void rotateClockwise();
    void rotateAntiClockwise();
};

#endif // GRID_H

