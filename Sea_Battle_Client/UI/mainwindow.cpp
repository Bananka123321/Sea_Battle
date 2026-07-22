#include "mainwindow.h"

#include <QRadialGradient>
#include <QMessageBox>
#include <QTimer>

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

    shootSound = new QSoundEffect(this);
    killSound = new QSoundEffect(this);
    missSound = new QSoundEffect(this);
    hitSound = new QSoundEffect(this);
    shootSound->setSource(QUrl::fromLocalFile(":/Effects/Sounds/shoot.wav"));
    killSound->setSource(QUrl::fromLocalFile(":/Effects/Sounds/kill.wav"));
    missSound->setSource(QUrl::fromLocalFile(":/Effects/Sounds/miss.wav"));
    hitSound->setSource(QUrl::fromLocalFile(":/Effects/Sounds/hit.wav"));

    createShips();


    connect(ui->CreateLobbyPushButton, &QPushButton::clicked, this, [this](){
        tryCreateLobby();
    });

    connect(ui->ConnectLobbyPushButton, &QPushButton::clicked, this, [this](){
        tryJoinLobby();
    });

    connect(ui->SendMessagePushButton, &QPushButton::clicked, this, &MainWindow::onSendChatMessage);
    connect(ui->SendMessageLineEdit, &QLineEdit::returnPressed, this, &MainWindow::onSendChatMessage);
    connect(ui->SendMessageInGamePushButton, &QPushButton::clicked, this, &MainWindow::onSendChatMessage);
    connect(ui->SendMessageInGameLineEdit, &QLineEdit::returnPressed, this, &MainWindow::onSendChatMessage);

    connect(ui->RandomSetPushButton, &QPushButton::clicked, placementBoard_, &PlacementBoard::randomPlacement);

    connect(enemyBoard_, &GameBoard::cellClicked, this, &MainWindow::enemyCellClicked);

    QTimer::singleShot(0, this, [this]()
    {
        resizeWindow();
    });

    ui->stackedWidget->setCurrentWidget(ui->ConnectPage);
}

MainWindow::~MainWindow() {
    delete placementBoard_;
    delete ownBoard_;
    delete enemyBoard_;
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    resizeWindow();
}

void MainWindow::resizeWindow()
{
    ui->graphicsView->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
    ui->ownGraphicsView->fitInView(ownScene_->sceneRect(), Qt::KeepAspectRatio);
    ui->enemyGraphicsView->fitInView(enemyScene_->sceneRect(), Qt::KeepAspectRatio);

    ui->graphicsView->scale(0.8, 0.8);
    ui->ownGraphicsView->scale(0.8, 0.8);
    ui->enemyGraphicsView->scale(0.8, 0.8);
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

std::vector<ShipData> MainWindow::convertShips() {
    std::vector<PlacementBoard::ShipData> shipsToPlace(placementBoard_->getShips().constBegin(), placementBoard_->getShips().constEnd());

    std::vector<ShipData> networkShips;

    networkShips.reserve(shipsToPlace.size());

    for (const auto& uiShip : shipsToPlace) {
        ShipData netShip;

        netShip.row = uiShip.row;
        netShip.column = uiShip.col;
        netShip.size = uiShip.size;
        netShip.horizontal = uiShip.horizontal;

        networkShips.push_back(netShip);
    }

    return networkShips;
}

void MainWindow::on_ReadyPushButton_clicked() {
    if (!placementBoard_->allShipPlaced())
    {
        QMessageBox::warning(this, "Ошибка", "Расставьте все корабли.");
        return;
    }

    ui->graphicsView->setEnabled(!ui->graphicsView->isEnabled());
    ui->RandomSetPushButton->setEnabled(!ui->RandomSetPushButton->isEnabled());

    emit playerReady(convertShips());

    if (ui->graphicsView->isEnabled() == false)
    {
        ui->ReadyPushButton->setText("Отмена");
    }
    else
    {
        ui->ReadyPushButton->setText("Готов");
    }
}

void MainWindow::drawShipsOnOwnBoard()
{
    for(auto ship : placementBoard_->getShips())
    {
        ownBoard_->addShip(ship.row, ship.col, ship.size, ship.horizontal);
    }
}

void MainWindow::tryCreateLobby() {
    std::string username = ui->ConnectUserNameLineEdit->text().toStdString();

    if(auto err = Validator::username(username)) return;

    nameClient_ = QString::fromStdString(username);
    emit createLobbyRequest(username);
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

    nameClient_ = QString::fromStdString(username);
    emit joinLobbyRequest(username, code);
}

Ui::MainWindow* MainWindow::getUI() const {
    return ui;
}

QGraphicsScene* MainWindow::getPlaceScene()
{
    return scene_;
}

QGraphicsScene* MainWindow::getOwnScene()
{
    return ownScene_;
}

ClickableScene* MainWindow::getEnemyScene()
{
    return enemyScene_;
}

void MainWindow::setPlayerState(Ui::PlayerState state) {
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
    if (!myTurn_) {
        qDebug() << "Сейчас не твой ход!";
        return;
    }

    if (enemyBoard_->getImage(row, col)->data(0).toString() != "untouch")
    {
        qDebug() << "Эта клетка недоступна!";
        return;
    }

    shootSound->play();

    emit shootRequest(row, col);

    ui->enemyGraphicsView->setEnabled(false);
}

bool MainWindow::getYourTurn() {
    return myTurn_;
}

void MainWindow::setYourTurn(bool yourTurn) {
    myTurn_ = yourTurn;

    ui->enemyGraphicsView->setEnabled(myTurn_);;

    if (yourTurn) {
        ui->MoveLabel->setText("Ваш ход!");
    } else {
        ui->MoveLabel->setText("Ход противника!");
    }
}

void MainWindow::shootResultEnemy(int row, int column, int status, bool shipSunk, const std::vector<std::pair<int, int>>& shipCells) {
    if (status == 0) {
        enemyBoard_->shootAtCell(row, column, Action::Miss);
        missSound->play();
    }
    else if (status == 1) {
        enemyBoard_->shootAtCell(row, column, Action::Hit);
        hitSound->play();
    }
    else if (status == 2) {
        enemyBoard_->shootAtCell(row, column, Action::Hit);
        enemyBoard_->markCellAsKill(shipCells);
        killSound->play();
    }
    else if (status == 3) {
        enemyBoard_->shootAtCell(row, column, Action::Hit);
    }
}

void MainWindow::shootResultMe(int row, int column, int status, bool shipSunk, const std::vector<std::pair<int, int>>& shipCells) {
    if (status == 0) {
        ownBoard_->shootAtCell(row, column, Action::Miss);
    }
    else if (status == 1) {
        ownBoard_->shootAtCell(row, column, Action::Hit);
    }
    else if (status == 2) {
        ownBoard_->shootAtCell(row, column, Action::Hit);    
        ownBoard_->markCellAsKill(shipCells);
    }
    else if (status == 3) {
        ownBoard_->shootAtCell(row, column, Action::Hit);
    }
}

void MainWindow::onSendChatMessage() {
    QLineEdit* activeLineEdit = nullptr;

    QObject* obj = sender();
    if (obj == ui->SendMessageLineEdit || obj == ui->SendMessagePushButton) {
        activeLineEdit = ui->SendMessageLineEdit;
    }
    else if (obj == ui->SendMessageInGameLineEdit || obj == ui->SendMessageInGamePushButton) {
        activeLineEdit = ui->SendMessageInGameLineEdit;
    }

    if (!activeLineEdit) return;

    QString text = activeLineEdit->text();
    if (text.isEmpty()) return;

    std::string cleanText = Validator::sanitize(text.toStdString());
    if (cleanText.empty()) {
        activeLineEdit->clear();
        return;
    }

    emit sendMessageRequest(cleanText);

    appendMessageToChat(ui->ChatInGameTextEdit, "Вы", cleanText, true);
    appendMessageToChat(ui->ChatTextEdit, "Вы", cleanText, true);

    activeLineEdit->clear();
}

void MainWindow::onReceiveChatMessage(const std::string& from, const std::string& text) {
    QString fromStr = QString::fromStdString(from);

    appendMessageToChat(ui->ChatTextEdit, fromStr, text, false);
    appendMessageToChat(ui->ChatInGameTextEdit, fromStr, text, false);
}

void MainWindow::appendMessageToChat(QTextEdit* chatView, const QString& name, const std::string& text, bool isMe) {
    if (!chatView) return;

    QString align     = isMe ? "right" : "left";
    QString bgColor   = isMe ? "#89b4fa" : "#313244";
    QString textColor = isMe ? "#11111b" : "#cdd6f4";
    QString timeColor = isMe ? "#45475a" : "#a6adc8";
    QString timeStr   = QDateTime::currentDateTime().toString("HH:mm");

    QString escapedText = QString::fromStdString(text).toHtmlEscaped();
    escapedText.replace("\n", "<br>");

    QString html = QString(
                       "<table width='100%' style='margin: 4px 0px; border: none;'>"
                       "  <tr>"
                       "    <td align='%1' style='border: none; padding: 0px;'>"
                       "      <div style='color: %5; font-size: 10px; margin-bottom: 2px;'>%7</div>"
                       "      <table style='background-color: %2; border-radius: 12px; margin: 0px; border: none;' cellpadding='8' cellspacing='0'>"
                       "        <tr>"
                       "          <td style='border: none; padding: 6px 10px 4px 10px; max-width: 450px;'>"
                       "            <span style='color: %3; font-size: 13px; font-family: \"Segoe UI\", sans-serif;'>%4</span>"
                       "          </td>"
                       "        </tr>"
                       "        <tr>"
                       "          <td align='right' style='border: none; padding: 0px 10px 4px 10px;'>"
                       "            <span style='color: %5; font-size: 9px; font-family: \"Segoe UI\", sans-serif;'>%6</span>"
                       "          </td>"
                       "        </tr>"
                       "      </table>"
                       "    </td>"
                       "  </tr>"
                       "</table>"
                       ).arg(align, bgColor, textColor, escapedText, timeColor, timeStr, name);

    QTextCursor cursor(chatView->document());
    cursor.movePosition(QTextCursor::End);
    cursor.insertHtml(html);

    chatView->verticalScrollBar()->setValue(chatView->verticalScrollBar()->maximum());
}

void MainWindow::revenge()
{
    ui->stackedWidget->setCurrentWidget(ui->LobbyPage);
    emit revengeRequest();
}

void MainWindow::clear()
{
    ownBoard_->clear();
    enemyBoard_->clear();

    for (ShipItem *ship : placementBoard_->getAllShips())
    {
        ship->returnSpawn();
    }

    placementBoard_->clear();


    ui->graphicsView->setEnabled(true);
    ui->RandomSetPushButton->setEnabled(true);
    ui->ReadyPushButton->setText("Готов");
    ui->OPConnectLabel->setText("Оппонент не готов");
}

void MainWindow::gameOver(const std::string& winner, std::vector<ShipData> opponentShips) {
    QMessageBox msgBox(this);

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.button(QMessageBox::Yes)->setText("Реванш");
    msgBox.button(QMessageBox::No)->setText("В меню");

    if (winner == nameClient_)
    {
        msgBox.setWindowTitle("Победа!");
        msgBox.setText("Вы потопили все корабли противника!");
    }
    else
    {
        enemyBoard_->showAllShips(opponentShips);
        msgBox.setWindowTitle("Поражение");
        msgBox.setText("Противник потопил все ваши корабли!");
    }
    msgBox.exec();


    if (msgBox.clickedButton() == msgBox.button(QMessageBox::Yes))
    {
        revenge();
    }
    else
    {
        ui->stackedWidget->setCurrentWidget(ui->ConnectPage);
        emit leaveLobbyRequest();
    }
    clear();
}
