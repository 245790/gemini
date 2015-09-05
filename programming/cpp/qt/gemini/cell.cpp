#include "cell.h"

Cell::Cell()
{
    status = DEAD;
    cellAge = 0;
}

Cell::Cell(int _status)
{
    cellAge = 0;
    if(_status == 1)
    {
      status = ALIVE;
    }
    else
    {
      status = DEAD;
    }
}

void Cell::setStatus(cellStatus s)
{
    if((int)s >= 0 && (int)s <=3)
    {
        status = s;
    }
}

cellStatus Cell::getStatus()
{
    return status;
}

int Cell::getAge()
{
    return cellAge;
}

void Cell::incrementAge()
{
    if(cellAge < 3)
    {
        cellAge++;
    }
}

void Cell::nullifyAge()
{
    cellAge = 0;
}
