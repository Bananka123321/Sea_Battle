#pragma once

#include "./ui_mainwindow.h"

#include <QMainWindow>
#include <QResizeEvent>
#include <QGraphicsScene>
#include <cmath>
#include <QDebug>
#include <QColor>
#include <QDateTime>
#include <QScrollBar>
#include <QSoundEffect>
#include <QMessageBox>

#include "Message.h"
#include "board.h"
#include "shipitem.h"
#include "gameboard.h"
#include "Validator.h"
#include "clickablescene.h"
#include "ship_coord.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;

enum class PlayerState {
    NOT_PLAYER,
    PLAYER_NOT_READY,
    PLAYER_READY
};

}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow* getUI() const;
    QGraphicsScene* getPlaceScene();
    QGraphicsScene* getOwnScene();
    ClickableScene* getEnemyScene();
    void resizeWindow();

    void setPlayerState(Ui::PlayerState state, const std::string& username = "");
    bool getYourTurn();
    void setYourTurn(bool yourTurn);

    void shootResultEnemy(int row, int column, int status, bool shipSunk, const std::vector<std::pair<int, int>>& shipCells);
    void shootResultMe(int row, int column, int status, bool shipSunk, const std::vector<std::pair<int, int>>& shipCells);
public slots:
    void onReceiveChatMessage(const std::string& from, const std::string& text);
    void gameOver(const std::string& winner, std::vector<ShipData> opponentShips);

private slots:
    void shipPlaced(ShipItem *ship, int row, int col, int size, bool horizotnal);

    void on_ReadyPushButton_clicked();

signals:
    void sendMessageRequest(const std::string& text);
    void createLobbyRequest(const std::string& username);
    void joinLobbyRequest(const std::string& username, const std::string& code);
    void playerReady(const std::vector<ShipData>& ships);
    void shootRequest(int row, int column);
    void revengeRequest();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene_;

    QGraphicsScene *ownScene_;
    ClickableScene *enemyScene_;

    GameBoard *ownBoard_;
    GameBoard *enemyBoard_;

    PlacementBoard* placementBoard_ = nullptr;
    Ui::PlayerState state_;

    bool myTurn_ = false;

    QSoundEffect* shootSound;
    QSoundEffect* killSound;
    QSoundEffect* missSound;
    QSoundEffect* hitSound;

private:
    void createShips();
    void addShip(ShipItem *ship, int x, int y);

    void refreshChat();

    void appendMessageToView(const Message& msg);//Используем для вывода 1 сообщения

    QString formatTime(int64_t timestamp);
    QString buildMessageHtml(const Message& msg);

    void onSendChatMessage();
    void appendMessageToChat(QTextEdit* chatView, const QString& name, const std::string& text, bool isMe);

    void tryCreateLobby();
    void tryJoinLobby();

    void enemyCellClicked(int row,int col);

    std::vector<ShipData> convertShips();

    void revenge();
};

