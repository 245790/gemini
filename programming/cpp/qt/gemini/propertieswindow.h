/* KPCC
 * Widnow with various information about the field, e. g. number of living cells, population etc.
 * Author: Safin Karim
 * Date: 2016.02.17
 */

#ifndef PROPERTIESWINDOW
#define PROPERTIESWINDOW

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include "gridpainter.h"

class PropertiesWindow : public QWidget
{
    Q_OBJECT

private:
    QLabel *generation;
    QLabel *population;
    QVBoxLayout *mainLayout;

public:
    void setGeneration(int gen);
    void setPopulation(int pop);
    PropertiesWindow(QWidget *parent = 0);
    ~PropertiesWindow();
};

#endif // PROPERTIESWINDOW
