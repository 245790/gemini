#include "userinterface.h"
#include <QtWidgets>

UserInterface::UserInterface()
{

    all = new QWidget;
    setCentralWidget(all);

    gridPainter = new GridPainter(this);

    stopButton = new QPushButton(tr("Stop"));
    connect(stopButton, SIGNAL(pressed()), gridPainter, SLOT(stopPressed()));

    mainLayout = new QVBoxLayout;
    mainLayout->setMargin(5);
    mainLayout->addWidget(gridPainter);
    mainLayout->addWidget(stopButton);

    all->setLayout(mainLayout);

    createActions();
    createMenus();

    setWindowTitle(tr("Conway's game of Life"));

    resize(640, 480);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), gridPainter, SLOT(animate()));
    timer->start(50);
}

void UserInterface::createActions()
{
    setCellColorAct = new QAction(tr("Set &cell color"), this);
    connect(setCellColorAct, SIGNAL(triggered()), this, SLOT(setCellColor()));

    setSpaceColorAct = new QAction(tr("Set &space color"), this);
    connect(setSpaceColorAct, SIGNAL(triggered()), this, SLOT(setSpaceColor()));
}

void UserInterface::createMenus()
{
    menuBar = new QMenuBar();

    viewMenu = new QMenu(tr("&View"));
    viewMenu->addAction(setCellColorAct);
    viewMenu->addAction(setSpaceColorAct);
    viewMenu->addSeparator();

    menuBar->addMenu(viewMenu);

    setMenuBar(menuBar);
}

void UserInterface::setCellColor()
{
    gridPainter->setCellColor(QColorDialog::getColor(Qt::green,
                                                    this,
                                                    "Select cell color"));
}

void UserInterface::setSpaceColor()
{
    gridPainter->setSpaceColor(QColorDialog::getColor(Qt::white,
                                                    this,
                                                    "Select space color"));
}

UserInterface::~UserInterface()
{

}
