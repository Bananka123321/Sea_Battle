#include "mainwindow.h"

#include <QRadialGradient>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene_);

    placementBoard_ = new PlacementBoard(scene_);
    placementBoard_->drawField();

    ownScene_ = new QGraphicsScene(this);
    ui->ownGraphicsView->setScene(ownScene_);

    enemyScene_ = new ClickableScene(this);
    ui->enemyGraphicsView->setScene(enemyScene_);

    ui->ownGraphicsView->setAlignment(Qt::AlignCenter);
    ui->enemyGraphicsView->setAlignment(Qt::AlignCenter);

    scene_->setSceneRect(0, 0, 400, 400);
    ownScene_->setSceneRect(0, 0, 400, 400);
    enemyScene_->setSceneRect(0, 0, 400, 400);

    ownBoard_ = new GameBoard(ownScene_);
    enemyBoard_ = new GameBoard(enemyScene_);

    ownBoard_->drawField();
    ownBoard_->drawImage();
    enemyBoard_->drawField();
    enemyBoard_->drawImage();

    enemyBoard_->setCellImage(4,4,":/ships/tail_H_ship.png");
    enemyBoard_->setCellImage(4,5,":/ships/middle_H34_ship.png");
    enemyBoard_->setCellImage(4,6,":/ships/middle_H4_ship.png");
    enemyBoard_->setCellImage(4,7,":/ships/head_H_ship.png");


    createShips();

    connect(ui->CreateLobbyPushButton, &QPushButton::clicked, this, [this](){
        tryCreateLobby();
    });

    connect(ui->ConnectLobbyPushButton, &QPushButton::clicked, this, [this](){
        tryJoinLobby();
    });
    connect(ui->RandomSetPushButton, &QPushButton::clicked, placementBoard_, &PlacementBoard::randomPlacement);

    connect(enemyBoard_, &GameBoard::cellClicked, this, &MainWindow::enemyCellClicked);

    ui->stackedWidget->setCurrentWidget(ui->ConnectPage);
}

MainWindow::~MainWindow() {
    delete ui;
}

static int SPAWNX = OFFSET + 270;
static int SPAWNY = OFFSET + 30;

void MainWindow::addShip(ShipItem *ship, int x, int y)
{
    placementBoard_->registerShip(ship);

    connect(ship, &ShipItem::placed, this, &MainWindow::shipPlaced);

    connect(ship, &ShipItem::requestRotate, this, [this](ShipItem *ship)
            {
                ship->rotate();

                QPointF p = ship->pos();

                int row = qRound((p.y() - OFFSET) / CELL_SIZE);
                int col = qRound((p.x() - OFFSET) / CELL_SIZE);

                if (row < -1 || row > 10 || col < -1 || col > 10)
                {
                    return;
                }

                row = qBound(0, row, 9);
                col = qBound(0, col, 9);

                if (ship->isHorizontal())
                {
                    col = qMin(col, 10 - ship->size());
                }
                else
                {
                    row = qMin(row, 10 - ship->size());
                }

                if (placementBoard_->canPlaceShip(ship, row, col, ship->size(), ship->isHorizontal()))
                {
                    placementBoard_->addShip(ship, row, col, ship->size(), ship->isHorizontal());
                    ship->setPos(OFFSET + col * CELL_SIZE, OFFSET + row * CELL_SIZE);
                }
            });

    connect(ship, &ShipItem::removedFromBoard, this, [this](ShipItem *ship)
            {
                placementBoard_->removeShip(ship);
            });

    connect(ship, &ShipItem::dragStarted, this, [this](ShipItem *ship)
            {
                placementBoard_->showForbiddenZones(ship);
            });

    connect(ship, &ShipItem::dragFinished, this, [this]()
            {
                placementBoard_->hideForbiddenZones();
            });

    ship->setPos(x, y);
    ship->setSpawnPos(QPointF(x, y));
    ship->clearFocus();

    scene_->addItem(ship);
}

void MainWindow::createShips() {
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

void MainWindow::shipPlaced(ShipItem *ship, int row, int col, int size, bool horizontal) {
    qDebug() << "Ship placed:"
             << row
             << col
             << size
             << horizontal;

    if (row < -1 || row > 10 || col < -1 || col > 10)
    {
        ship->restoreState();
        return;
    }

    row = qBound(0, row, 9);
    col = qBound(0, col, 9);

    if (horizontal)
    {
        col = qMin(col, 10 - size);
    }
    else
    {
        row = qMin(row, 10 - size);
    }

    if (placementBoard_->canPlaceShip(ship, row, col, size, horizontal))
    {
        placementBoard_->addShip(ship, row, col, size, horizontal);

        ship->setPos(OFFSET + col*CELL_SIZE, OFFSET + row*CELL_SIZE);
        ship->setFocus();
    }
    else
    {
        ship->restoreState();
    }
}

void MainWindow::tryCreateLobby() {
    std::string username = ui->ConnectUserNameLineEdit->text().toStdString();

    if(auto err = Validator::username(username)) return;

    emit createLobbyRequest(username);
}


void MainWindow::on_ReadyPushButton_clicked() {
    if (!placementBoard_->allShipPlaced())
    {
        QMessageBox::warning(this, "Ошибка", "Расставьте все корабли.");
        return;
    }

    ui->graphicsView->setEnabled(!ui->graphicsView->isEnabled());
    ui->RandomSetPushButton->setEnabled(!ui->RandomSetPushButton->isEnabled());

    std::vector<PlacementBoard::ShipData> shipsToPlace = placementBoard_->getShip().toVector();

    emit playerReady(shipsToPlace);

    if (ui->graphicsView->isEnabled() == false)
    {
        ui->ReadyPushButton->setText("Отмена");
    }
    else
    {
        ui->ReadyPushButton->setText("Готов");
    }

    for(auto ship : placementBoard_->getShip())
    {
        ownBoard_->addShip(ship.row, ship.col, ship.size, ship.horizontal);
    }

    ui->stackedWidget->setCurrentWidget(ui->GamePage);
}

void MainWindow::tryJoinLobby() {
    std::string username = ui->ConnectUserNameLineEdit->text().toStdString();
    std::string code = ui->ConnectCodeRoomLineEdit->text().toStdString();

    if(auto err = Validator::username(username))
    {
        return;
    }
    if(auto err = Validator::username(code))
    {
        return;
    }

    ui->CodeRoomLabel->setText(QString::fromStdString(code));

    emit joinLobbyRequest(username, code);
}

Ui::MainWindow* MainWindow::getUI() const {
    return ui;
}

void MainWindow::setPlayerState(Ui::PlayerState state, const std::string& username) {
    QLabel* enemy = ui->OPConnectLabel;
    switch (state) {
    case Ui::PlayerState::NOT_PLAYER:
        enemy->setText("Оппонент не подключился");
        break;
    case Ui::PlayerState::PLAYER_NOT_READY:
        enemy->setText("Оппонент не готов");
        break;
    case Ui::PlayerState::PLAYER_READY:
        enemy->setText("Оппонент готов!");
        break;
    }
}

void MainWindow::enemyCellClicked(int row, int col)
{
    enemyBoard_->shootAtCell(row, col, Action::Hit);
    qDebug() << "Hit: " << row << " " << col;
}
