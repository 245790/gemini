/* KPCC
 * Graphical user interface
 * Author: Alexandra Balyuk
 * Date: 2015.09.05
 */

#include <QDebug>

#include <QString>
#include <QtWidgets>

#include "userinterface.h"

UserInterface::UserInterface()
{

    all = new QWidget;
    setCentralWidget(all); // Central widget of a widnow is "all"

    gridPainter = new GridPainter(this);

    stopButton = new QPushButton(tr("Start"));
    connect(stopButton, SIGNAL(pressed()), this, SLOT(stopButtonPressed()));

    clearButton = new QPushButton(tr("Clear"));
    connect(clearButton, SIGNAL(pressed()), gridPainter, SLOT(clear()));

    nextGenerationButton = new QPushButton(tr("Next Generation"));
    connect(nextGenerationButton, SIGNAL(pressed()), gridPainter, SLOT(nextGeneration()));

    mode = new QTreeView;
    mode->setModel(modelFromFile(":/model.txt")); // Resources/...
    mode->setMaximumWidth(400);
    connect(mode, SIGNAL(clicked(QModelIndex)), this, SLOT(changeMode(QModelIndex)));

    painterAndMode = new QHBoxLayout;
    painterAndMode->addWidget(mode);
    painterAndMode->addWidget(gridPainter, 1);
    // 1 means that gridPainter gets as big as possible when the window is stretched

    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(stopButton);
    mainLayout->addWidget(clearButton);
    mainLayout->addWidget(nextGenerationButton);

    layout = new QVBoxLayout;
    layout->addLayout(painterAndMode);
    layout->addLayout(mainLayout);

    all->setLayout(layout);

    propertiesWindow = new PropertiesWindow();

    createActions();
    createMenus();

    setWindowTitle(tr("Conway's game of Life"));

    resize(640, 480);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), gridPainter, SLOT(animate()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePropertiesWindow()));
    timer->start(100);
}

void UserInterface::createActions()
{
    openRleFileAct = new QAction(tr("&Open rle file"), this);
    connect(openRleFileAct, SIGNAL(triggered()), this, SLOT(openRleFile()));

    openPlainTextFileAct = new QAction(tr("Open &plain text file"), this);
    connect(openPlainTextFileAct, SIGNAL(triggered()), this, SLOT(openPlainTextFile()));

    initRandomAct = new QAction(tr("&Fill with random data"), this);
    connect(initRandomAct, SIGNAL(triggered()), this, SLOT(initRandom()));

    setCellColorAct = new QAction(tr("Set &cell color"), this);
    connect(setCellColorAct, SIGNAL(triggered()), this, SLOT(setCellColor()));

    setSpaceColorAct = new QAction(tr("Set &space color"), this);
    connect(setSpaceColorAct, SIGNAL(triggered()), this, SLOT(setSpaceColor()));

    setGridColorAct = new QAction(tr("Set &grid color"), this);
    connect(setGridColorAct, SIGNAL(triggered()), this, SLOT(setGridColor()));

    chooseWhiteThemeAct = new QAction(tr("Choose &white theme"), this);
    connect(chooseWhiteThemeAct, SIGNAL(triggered()), this, SLOT(chooseWhiteTheme()));

    chooseBlackThemeAct = new QAction(tr("Choose &black theme"), this);
    connect(chooseBlackThemeAct, SIGNAL(triggered()), this, SLOT(chooseBlackTheme()));

    setUpdateRateAct = new QAction(tr("Set re&fresh rate"), this);
    connect(setUpdateRateAct, SIGNAL(triggered()), this, SLOT(setUpdateRate()));

    rotateClockwiseAct = new QAction(tr("&Rotate clock wise"), this);
    connect(rotateClockwiseAct, SIGNAL(triggered()), gridPainter, SLOT(rotateClockwise()));

    rotateAntiClockwiseAct = new QAction(tr("Rotate &anti clock wise"), this);
    connect(rotateAntiClockwiseAct, SIGNAL(triggered()), gridPainter, SLOT(rotateAntiClockwise()));
}

void UserInterface::createMenus()
{
    menuBar = new QMenuBar();

    fileMenu = new QMenu(tr("&File"));
    fileMenu->addAction(openRleFileAct);
    fileMenu->addAction(openPlainTextFileAct);


    viewMenu = new QMenu(tr("&View"));
    viewMenu->addAction(setCellColorAct);
    viewMenu->addAction(setSpaceColorAct);
    viewMenu->addAction(setGridColorAct);
    viewMenu->addSeparator();
    viewMenu->addAction(chooseWhiteThemeAct);
    viewMenu->addAction(chooseBlackThemeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(setUpdateRateAct);

    editMenu = new QMenu(tr("&Edit"));
    editMenu->addAction(initRandomAct);
    editMenu->addAction(rotateClockwiseAct);
    editMenu->addAction(rotateAntiClockwiseAct);


    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
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
            UserInterface::drawingIndex = rowCount; // i mean the class variable
        }

        if(line.trimmed().toLower() == "erasing")
        {
            readErasing = true;
            UserInterface::erasingIndex = rowCount; // i mean the class variable
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

void UserInterface::setGridColor()
{
    if(!gridPainter->isStopped())
    {
        stopButtonPressed();
    }
    gridPainter->setGridColor(QColorDialog::getColor(Qt::white,
                                                    this,
                                                    "Select grid color"));
    gridPainter->update();
}

void UserInterface::chooseWhiteTheme()
{
    gridPainter->setCellColor(QColor(0, 0, 0));
    gridPainter->setSpaceColor(QColor(240, 240, 240));
    gridPainter->setGridColor(QColor(230, 230, 230));
    gridPainter->update();
}

void UserInterface::chooseBlackTheme()
{
    gridPainter->setCellColor(QColor(255, 255, 255));
    gridPainter->setSpaceColor(QColor(0, 0, 0));
    gridPainter->setGridColor(QColor(64, 64, 64));
    gridPainter->update();
}

void UserInterface::setUpdateRate()
{
    timer->setInterval(QInputDialog::getInt(this,
                                            tr("Enter refresh rate(miliseconds)"),
                                            tr("Enter refresh rate(miliseconds)"),
                                            100,
                                            1,
                                            1000000,
                                            1));
}

void UserInterface::openRleFile()
{
    if(!gridPainter->isStopped())
    {
        stopButtonPressed();
    }

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open RLE file"));

    gridPainter->parseRLE(fileName);

    gridPainter->rotateClockwise(); // There is a bug in parsing programs, fixed by rotation

    gridPainter->update();

    setWindowTitle(fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1) + tr(" - Conway's game of Life"));
}

void UserInterface::openPlainTextFile()
{
    if(!gridPainter->isStopped())
    {
        stopButtonPressed();
    }

    QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Open plain text file"));
    gridPainter->parsePlainText(fileName);

    gridPainter->rotateClockwise(); // There is a bug in parsing programs, fixed by rotation

    gridPainter->update();

    setWindowTitle(fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1) + tr(" - Conway's game of Life"));
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
    if(index.row() == 0) // then the user clicked on MOVING
    {
        gridPainter->setMouseMode(MOVING);
        return;
    }
    if(index.row() >= drawingIndex && index.row() < erasingIndex) // then the user clicked on DRAWING
    {
        gridPainter->setMouseMode(DRAWING);
        if(index.row() != drawingIndex)
        {
            gridPainter->setCurrentDrawingPattern(index.row() - drawingIndex - 1);
        }
        return;
    }
    if(index.row() > erasingIndex) // then the user clicked on DRAWING
    {
        gridPainter->setMouseMode(ERASING);
        if(index.row() != erasingIndex)
        {
            gridPainter->setCurrentErasingPattern(index.row() - erasingIndex - 1);
        }
        return;
    }
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

void UserInterface::updatePropertiesWindow()
{
    propertiesWindow->setGeneration(gridPainter->getGenerationCount());
    propertiesWindow->setPopulation(gridPainter->getPopulation());
}

void UserInterface::keyPressEvent(QKeyEvent * event)
{
    switch(event->key())
    {
    case Qt::Key_V:
        gridPainter->setMouseMode(MOVING);
        break;
    case Qt::Key_D:
        gridPainter->setMouseMode(DRAWING);
        break;
    case Qt::Key_E:
        gridPainter->setMouseMode(ERASING);
        break;
    case Qt::Key_Space:
        gridPainter->stopPressed();
        break;
    case Qt::Key_N:
        gridPainter->nextGeneration();
        break;
    case Qt::Key_0:
        gridPainter->setCurrentPattern(0);
        break;
    case Qt::Key_1:
        gridPainter->setCurrentPattern(1);
        break;
    case Qt::Key_2:
        gridPainter->setCurrentPattern(2);
        break;
    case Qt::Key_3:
        gridPainter->setCurrentPattern(3);
        break;
    case Qt::Key_4:
        gridPainter->setCurrentPattern(4);
        break;
    case Qt::Key_5:
        gridPainter->setCurrentPattern(5);
        break;
    case Qt::Key_6:
        gridPainter->setCurrentPattern(6);
        break;
    case Qt::Key_7:
        gridPainter->setCurrentPattern(7);
        break;
    case Qt::Key_8:
        gridPainter->setCurrentPattern(8);
        break;
    case Qt::Key_9:
        gridPainter->setCurrentPattern(9);
        break;
    }
    update();
}

UserInterface::~UserInterface()
{
}
