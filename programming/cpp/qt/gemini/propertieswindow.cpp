/* KPCC
 * Widnow with various information about the field, e. g. number of living
 * cells, population etc.
 * Author: Safin Karim
 * Date: 2016.02.17
 */

#include "propertieswindow.h"

PropertiesWindow::PropertiesWindow(QWidget *parent)
    : QWidget(parent)
{
    generationLabel= new QLabel(tr("Generation 0"));

    populationLabel = new QLabel(tr("Population: "));

    hashSizeLabel = new QLabel(tr("Items in hash: "));

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(generationLabel);
    mainLayout->addWidget(populationLabel);
    mainLayout->addWidget(hashSizeLabel);

    setWindowTitle(tr("Properties"));

    this->setLayout(mainLayout);
    this->show();
}

void PropertiesWindow::setGeneration(int gen)
{
    generationLabel->setText(tr("Generation ") + QString::number(gen));
}

void PropertiesWindow::setPopulation(int pop)
{
    populationLabel->setText(tr("Population: ") + QString::number(pop));
}

void PropertiesWindow::setHashSize(int hs)
{
    hashSizeLabel->setText(tr("Items in hash: ") + QString::number(hs));
}

PropertiesWindow::~PropertiesWindow()
{

}
