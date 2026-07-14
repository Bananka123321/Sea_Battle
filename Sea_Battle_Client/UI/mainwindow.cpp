#include "mainwindow.h"

#include <QRadialGradient>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene_);

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

    ownBoard_->draw();
    enemyBoard_->draw();

    enemyBoard_->shootAtCell(5,5, Action::Miss);
    enemyBoard_->shootAtCell(4,5, Action::Hit);
    enemyBoard_->shootAtCell(4,4,Action::Hit);

    createField();
    createShips();

    connect(ui->CreateLobbyPushButton, &QPushButton::clicked, this, [this](){
        tryCreateLobby();
    });

    connect(ui->ConnectLobbyPushButton, &QPushButton::clicked, this, [this](){
        tryJoinLobby();
    });

    connect(enemyBoard_, &GameBoard::cellClicked, this, &MainWindow::enemyCellClicked);

    // ui->stackedWidget->setCurrentWidget(ui->ConnectPage);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createField() {
    QPen pen(Qt::black);

    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            scene_->addRect(
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
    connect(ship, &ShipItem::requestRotate, this, [this](ShipItem* ship) {
        bool oldDirection = ship->isHorizontal();

        ship->rotate();

        int row = qRound(ship->pos().y()/CELL_SIZE);
        int col = qRound(ship->pos().x()/CELL_SIZE);

        if(board_.canPlaceShip(ship, row, col, ship->size(), ship->isHorizontal()))
            board_.addShip(ship, row, col, ship->size(), ship->isHorizontal());
        else
            ship->setDirection(oldDirection);
    });

    ship->setPos(x,y);

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

    if (board_.canPlaceShip(ship, row, col, size, horizontal)) {
        board_.addShip(ship, row, col, size, horizontal);

        ship->setPos(col*CELL_SIZE, row*CELL_SIZE);
    } else {
        ship->restoreState();
    }
}

void MainWindow::tryCreateLobby() {
    std::string username = ui->ConnectUserNameLineEdit->text().toStdString();

    if(auto err = Validator::username(username)) return;

    emit createLobbyRequest(username);
}


void MainWindow::on_ReadyPushButton_clicked() {
    ui->graphicsView->setEnabled(!ui->graphicsView->isEnabled());
    ui->RandomSetPushButton->setEnabled(!ui->RandomSetPushButton->isEnabled());

    emit playerReady();
    if (ui->graphicsView->isEnabled() == false)
        ui->ReadyPushButton->setText("Отмена");
    else
        ui->ReadyPushButton->setText("Готов");

    for(auto ship : board_.getShip())
    {
        ownBoard_->addShip(
            ship.row,
            ship.col,
            ship.size,
            ship.horizontal
            );
    }

    ui->stackedWidget->setCurrentWidget(ui->GamePage);
}

void MainWindow::tryJoinLobby() {
    std::string username = ui->ConnectUserNameLineEdit->text().toStdString();
    std::string code = ui->ConnectCodeRoomLineEdit->text().toStdString();

    if(auto err = Validator::username(username)) return;
    if(auto err = Validator::username(code)) return;

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
    qDebug() << "Hit: " << row << " " << col;
}
