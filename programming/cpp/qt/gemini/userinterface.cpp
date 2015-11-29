#include <QDebug>

#include <QString>
#include <QtWidgets>

#include "userinterface.h"

UserInterface::UserInterface()
{

    all = new QWidget;
    setCentralWidget(all);

    gridPainter = new GridPainter(this);

    stopButton = new QPushButton(tr("Start"));
    connect(stopButton, SIGNAL(pressed()), this, SLOT(stopButtonPressed()));

    clearButton = new QPushButton(tr("Clear"));
    connect(clearButton, SIGNAL(pressed()), gridPainter, SLOT(clear()));

    rotateClockwiseButton = new QPushButton(tr("Rotate clockwise"));
    connect(rotateClockwiseButton, SIGNAL(pressed()), gridPainter, SLOT(rotateClockwise()));

    rotateAntiClockwiseButton = new QPushButton(tr("Rotate anti-clockwise"));
    connect(rotateAntiClockwiseButton, SIGNAL(pressed()), gridPainter, SLOT(rotateAntiClockwise()));

    nextGenerationButton = new QPushButton(tr("Next Generation"));
    connect(nextGenerationButton, SIGNAL(pressed()), gridPainter, SLOT(nextGeneration()));

    mode = new QTreeView;
    mode->setModel(modelFromFile(":/model.txt"));
    mode->setMaximumWidth(400);
    connect(mode, SIGNAL(clicked(QModelIndex)), this, SLOT(changeMode(QModelIndex)));

    painterAndMode = new QHBoxLayout;
    painterAndMode->addWidget(mode);
    painterAndMode->addWidget(gridPainter, 1);

    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(stopButton);
    mainLayout->addWidget(clearButton);
    mainLayout->addWidget(rotateClockwiseButton);
    mainLayout->addWidget(rotateAntiClockwiseButton);
    mainLayout->addWidget(nextGenerationButton);

    layout = new QVBoxLayout;
    layout->addLayout(painterAndMode);
    layout->addLayout(mainLayout);

    all->setLayout(layout);

    createActions();
    createMenus();

    setWindowTitle(tr("Conway's game of Life"));

    resize(640, 480);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), gridPainter, SLOT(animate()));
    timer->start(100);
}

void UserInterface::createActions()
{
    openFileAct = new QAction(tr("&Open file"), this);
    connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFile()));

    initRandomAct = new QAction(tr("&Fill with random data"), this);
    connect(initRandomAct, SIGNAL(triggered()), this, SLOT(initRandom()));

    setCellColorAct = new QAction(tr("Set &cell color"), this);
    connect(setCellColorAct, SIGNAL(triggered()), this, SLOT(setCellColor()));

    setSpaceColorAct = new QAction(tr("Set &space color"), this);
    connect(setSpaceColorAct, SIGNAL(triggered()), this, SLOT(setSpaceColor()));

}

void UserInterface::createMenus()
{
    menuBar = new QMenuBar();

    fileMenu = new QMenu(tr("&File"));
    fileMenu->addAction(openFileAct);
    fileMenu->addAction(initRandomAct);

    viewMenu = new QMenu(tr("&View"));
    viewMenu->addAction(setCellColorAct);
    viewMenu->addAction(setSpaceColorAct);
    viewMenu->addSeparator();

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(viewMenu);

    setMenuBar(menuBar);
}

QAbstractItemModel *UserInterface::modelFromFile(const QString& fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&file);

    QStandardItemModel *model = new QStandardItemModel;

    int rowCount = 0;

    model->insertRow(rowCount);
    model->insertColumn(0);

    int paintingIndex = -1;
    bool readDrawing = false; // true if we met line "drawing" while parsing
    int erasingIndex = -1;
    bool readErasing = false; // true if we met line "erasing" while parsing

    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        model->setData(model->index(rowCount, 0), QVariant(line));

        if(line.trimmed().toLower() == "drawing")
        {
            readDrawing = true;
        }

        if(line.trimmed().toLower() == "erasing")
        {
            readErasing = true;
        }

        if(readDrawing && !readErasing)
        {
            if(paintingIndex >= 0)
            {
                gridPainter->setDrawingPattern(paintingIndex, line.trimmed());
            }
            paintingIndex++;
        }

        if(readDrawing && readErasing)
        {
            if(erasingIndex >= 0)
            {
                gridPainter->setErasingPattern(erasingIndex, line.trimmed());
            }
            erasingIndex++;
        }

        rowCount++;
        model->insertRow(rowCount);
    }

    return model;
}

void UserInterface::setCellColor()
{
    if(!gridPainter->isStopped())
    {
        stopButtonPressed();
    }
    gridPainter->setCellColor(QColorDialog::getColor(Qt::green,
                                                    this,
                                                    "Select cell color"));
    gridPainter->update();
}

void UserInterface::setSpaceColor()
{
    if(!gridPainter->isStopped())
    {
        stopButtonPressed();
    }
    gridPainter->setSpaceColor(QColorDialog::getColor(Qt::white,
                                                    this,
                                                    "Select space color"));
    gridPainter->update();
}
void UserInterface::openFile()
{
    if(!gridPainter->isStopped())
    {
        stopButtonPressed();
    }
    gridPainter->parseRLE(
                QFileDialog::getOpenFileName(this,
                                             tr("Open file")));
    gridPainter->update();
}

void UserInterface::initRandom()
{
    gridPainter->initRandom(QInputDialog::getInt(this,
                                                 tr("Enter width"),
                                                 tr("Enter width: "),
                                                 25,
                                                 5,
                                                 1000000,
                                                 1),
                            QInputDialog::getInt(this,
                                                 tr("Enter height"),
                                                 tr("Enter height: "),
                                                 25,
                                                 5,
                                                 1000000,
                                                 1));
    gridPainter->update();
}

void UserInterface::changeMode(const QModelIndex &index)
{
}

void UserInterface::stopButtonPressed()
{
    gridPainter->stopPressed();

    if(gridPainter->isStopped())
    {
        stopButton->setText("Start");
    }
    else
    {
        stopButton->setText("Stop");
    }
}

UserInterface::~UserInterface()
{

}
