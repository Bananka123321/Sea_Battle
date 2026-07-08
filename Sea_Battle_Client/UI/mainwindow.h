#pragma once

#include "./ui_mainwindow.h"

#include <QMainWindow>
#include <QResizeEvent>
#include <QGraphicsScene>
#include <cmath>
#include <QDebug>

#include "Message.h"
#include "board.h"
#include "shipitem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void shipPlaced(ShipItem *ship, int row, int col, int size, bool horizotnal);

signals:
    void sendMessageRequest(const int& to, const std::string& text);
    void searchUser(const std::string& text);
    void loadHistoryRequest(int peer_id, int last_msg_id);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    board board_;

private:
    void createField();
    void createShips();
    void addShip(ShipItem *ship, int x, int y);

    void refreshChat();

    void appendMessageToView(const Message& msg);//Используем для вывода 1 сообщения

    QString formatTime(int64_t timestamp);
    QString buildMessageHtml(const Message& msg);
};
