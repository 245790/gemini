/* KPCC
 * Widnow with various information about the field, e. g. number of living
 * cells, population etc.
 * File: propertieswindow.h
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
    QLabel *generationLabel;
    QLabel *populationLabel;
    QLabel *hashSizeLabel;
    QVBoxLayout *mainLayout;

public:
    void setGeneration(int gen);
    void setPopulation(int pop);
    void setHashSize(int hs);
    PropertiesWindow(QWidget *parent = 0);
    ~PropertiesWindow();
};

#endif // PROPERTIESWINDOW
