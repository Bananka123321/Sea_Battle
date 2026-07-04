#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "shipitem.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    scene->setSceneRect(0, 0, 400, 400);

    createField();
    createShips();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createField()
{
    QPen pen(Qt::black);

    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            scene->addRect(
                col * CELL_SIZE,
                row * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE,
                pen
                );
        }
    }
}

void MainWindow::createShips()
{
    ShipItem *ship1 = new ShipItem(4);
    ship1->setPos(450, 50);
    scene->addItem(ship1);

    ShipItem *ship2 = new ShipItem(3);
    ship2->setPos(450, 120);
    scene->addItem(ship2);

    ShipItem *ship3 = new ShipItem(2);
    ship3->setPos(450, 190);
    scene->addItem(ship3);
}

