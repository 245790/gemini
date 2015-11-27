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

    mode = new QListWidget;
    mode->setViewMode(QListView::IconMode);
    mode->setIconSize(QSize(30, 30));
    mode->setMovement(QListView::Static);
    mode->setMaximumWidth(128);
    mode->setSpacing(12);
    createIcons();

    painterAndMode = new QHBoxLayout;
    painterAndMode->addWidget(mode);
    painterAndMode->addWidget(gridPainter);

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

void UserInterface::createIcons()
{
    drawing = new QListWidgetItem(mode);
    drawing->setText(tr("Drawing"));
    drawing->setTextAlignment(Qt::AlignHCenter);
    drawing->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    moving = new QListWidgetItem(mode);
    moving->setText(tr("Moving"));
    moving->setTextAlignment(Qt::AlignHCenter);
    moving->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


    connect(mode,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changeMode(QListWidgetItem*,QListWidgetItem*)));

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

void UserInterface::changeMode(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    switch(mode->row(current))
    {
    case 0:
        gridPainter->setMouseMode(DRAWING);
    break;

    case 1:
        gridPainter->setMouseMode(MOVING);
    break;
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

UserInterface::~UserInterface()
{

}
