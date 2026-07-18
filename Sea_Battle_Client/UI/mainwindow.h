#pragma once

#include "./ui_mainwindow.h"

#include <QMainWindow>
#include <QResizeEvent>
#include <QGraphicsScene>
#include <cmath>
#include <QDebug>
#include <QColor>

#include "Message.h"
#include "board.h"
#include "shipitem.h"
#include "gameboard.h"
#include "Validator.h"
#include "clickablescene.h"

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
    void setPlayerState(Ui::PlayerState state, const std::string& username = "");

private slots:
    void shipPlaced(ShipItem *ship, int row, int col, int size, bool horizotnal);

    void on_ReadyPushButton_clicked();

signals:
    void sendMessageRequest(const int& to, const std::string& text);
    void createLobbyRequest(const std::string& username);
    void joinLobbyRequest(const std::string& username, const std::string& code);
    void playerReady(const std::vector<PlacementBoard::ShipData>& ships);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene_;

    QGraphicsScene *ownScene_;
    ClickableScene *enemyScene_;

    GameBoard *ownBoard_;
    GameBoard *enemyBoard_;


    PlacementBoard* placementBoard_ = nullptr;
    Ui::PlayerState state_;

private:
    void createField();
    void createShips();
    void addShip(ShipItem *ship, int x, int y);

    void refreshChat();

    void appendMessageToView(const Message& msg);//Используем для вывода 1 сообщения

    QString formatTime(int64_t timestamp);
    QString buildMessageHtml(const Message& msg);

    void tryCreateLobby();
    void tryJoinLobby();

    void enemyCellClicked(int row,int col);
};

