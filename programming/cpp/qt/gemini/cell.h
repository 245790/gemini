#ifndef CELL_H
#define CELL_H

enum cellStatus
{
    DEAD,
    ALIVE,
    WAS_DEAD,
    WAS_ALIVE
};

class Cell
{
private:
    cellStatus status;
    int cellAge;
public:
    Cell(); //initial status is DEAD
    Cell(int _status); //initialize a cell with stated status
    void setStatus(cellStatus s);
    cellStatus getStatus();
    int getAge();
    void incrementAge();
    void nullifyAge();
};

#endif // CELL_H
