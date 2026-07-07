#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <cmath>
#include <qDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    scene->setSceneRect(0, 0, 400, 400);

    createField();
    createShips();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createField() {
    QPen pen(Qt::black);

    qDebug() << CELL_SIZE;

    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            scene->addRect(
                col * CELL_SIZE,
                row * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE,
                pen);
        }
    }
}

static int SPAWNX = 270;
static int SPAWNY = 30;

void MainWindow::addShip(ShipItem *ship, int x, int y)
{
    connect(ship, &ShipItem::placed, this, &MainWindow::shipPlaced);
    connect(ship, &ShipItem::requestRotate, this, [this](ShipItem* ship)
    {
        bool oldDirection = ship->isHorizontal();

        ship->rotate();

        int row = qRound(ship->pos().y()/CELL_SIZE);
        int col = qRound(ship->pos().x()/CELL_SIZE);

        if(board_.canPlaceShip(ship, row, col, ship->size(), ship->isHorizontal()))
        {
            board_.addShip(ship, row, col, ship->size(), ship->isHorizontal());
        }
        else
        {
            ship->setDirection(oldDirection);

        }
    });

    ship->setPos(x,y);

    scene->addItem(ship);
}

void MainWindow::createShips()
{
    addShip(new ShipItem(4), SPAWNX, SPAWNY);

    addShip(new ShipItem(3), SPAWNX, SPAWNY + (1 + 1) * CELL_SIZE);
    addShip(new ShipItem(3), SPAWNX + (1 + 3) * CELL_SIZE, SPAWNY + (1 + 1) * CELL_SIZE);

    addShip(new ShipItem(2), SPAWNX, SPAWNY + (1 + 3) * CELL_SIZE);
    addShip(new ShipItem(2), SPAWNX + (1 + 2) * CELL_SIZE, SPAWNY + (1 + 3) * CELL_SIZE);
    addShip(new ShipItem(2), SPAWNX + (1 + 5) * CELL_SIZE, SPAWNY + (1 + 3) * CELL_SIZE);

    addShip(new ShipItem(1), SPAWNX, SPAWNY + (1 + 5) * CELL_SIZE);
    addShip(new ShipItem(1), SPAWNX + (1 + 1) * CELL_SIZE, SPAWNY + (1 + 5) * CELL_SIZE);
    addShip(new ShipItem(1), SPAWNX + (1 + 3) * CELL_SIZE, SPAWNY + (1 + 5) * CELL_SIZE);
    addShip(new ShipItem(1), SPAWNX + (1 + 5) * CELL_SIZE, SPAWNY + (1 + 5) * CELL_SIZE);
}

void MainWindow::shipPlaced(ShipItem *ship, int row, int col, int size, bool horizontal)
{
    qDebug() << "Ship placed:"
             << row
             << col
             << size
             << horizontal;

    if (board_.canPlaceShip(ship, row, col, size, horizontal))
    {
        board_.addShip(ship, row, col, size, horizontal);

        ship->setPos(col*CELL_SIZE, row*CELL_SIZE);
    }
    else
    {
        ship->restoreState();
    }
}
