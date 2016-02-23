/* KPCC
 * Widnow with various information about the field, e. g. number of living cells, population etc.
 * Author: Safin Karim
 * Date: 2016.02.17
 */

#include "propertieswindow.h"

PropertiesWindow::PropertiesWindow(QWidget *parent)
    : QWidget(parent)
{
    generation = new QLabel(tr("Generation 0"));

    population = new QLabel(tr("Population: "));

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(generation);
    mainLayout->addWidget(population);

    setWindowTitle(tr("Properties"));

    this->setLayout(mainLayout);
    this->show();
}

void PropertiesWindow::setGeneration(int gen)
{
    generation->setText(tr("Generation ") + QString::number(gen));
}

void PropertiesWindow::setPopulation(int pop)
{
    population->setText(tr("Population: ") + QString::number(pop));
}

PropertiesWindow::~PropertiesWindow()
{

}
