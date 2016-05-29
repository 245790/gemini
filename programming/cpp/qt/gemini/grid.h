/* KPCC
 * Class Grid is an interface for cellular automaton
 * File: grid.h
 * Author: Safin Karim
 * Date: 2014.12.04
 */

#ifndef GRID_H
#define GRID_H

#include "memory"
#include <QPainter>
#include <QString>

#include "treenode.h"

using namespace std;

class Grid
{
private:
    // WARNING! in this file, "grid" means "field", that is, automaton itself
    // while in gridpainter.h and .cpp "grid" means "lines like on graph paper"
    int generationCount; // number of a generation passed since creation
    shared_ptr<TreeNode> root; // actually a grid
public:

    // == initEmptyGrid(80, 25);
    Grid();

    // creates a square grid, whose side  = (width > height) ? width : height
    void initEmptyGrid(int width, int height);

    // fills a rectangle (width, height) with random cells
    void initRandom(int width, int height);

    // returns true if parsing is successful; false otherwise
    bool parsePlainText(const QString &fileName);

    // returns true if parsing is successful; false otherwise
    bool parseRLE(const QString &fileName);

    // Writes current field into file "fileName"
    void saveAsPlainText(const QString &fileName);
    void saveAsRLE(const QString &fileName);

    // kill all cells without any changes to the size of the grid
    void clear();

    // true if there are no living cells; false otherwise
    bool isEmpty() const;

    // true if root[heightIndex][widthIndex] is alive
    bool isAlive(int heightIndex, int widthIndex) const;

    void setAlive(int heightIndex, int widthIndex, bool isAlive);

    int getWidth() const;
    // returns the same as getWidth, as the field is square
    int getHeight() const;

    // calculates next generation and expands this if not all the cells fit
    void update();

    // draws itself so that (x0, y0) is in the center
    void draw(QPainter* painter, int x0, int y0, float width) const;

    // returns generationCount
    int getGeneration() const;

    // returns the number of living cells
    long getPopulation() const;

    // does what it reads
    void rotateClockwise();
    void rotateAntiClockwise();

    // left boundary - coordinate of the leftmost cell of a field, or 0
    // if all the cells are dead
    // other "boundaries" do the same for their direction
    int leftBoundary() const;
    int rightBoundary() const;
    int bottomBoundary() const;
    int topBoundary() const;

    // inserts the argument into this grid at the given position
    // if alive is true, then the inserted cells will be alive
    // otherwise - dead
    void insertPattern(const Grid& pattern, int xPos, int yPos, bool alive);

    // returns the array representation of this
    QVector<QVector<int> > as2dArray() const;

    int hashSize();
};

#endif // GRID_H
