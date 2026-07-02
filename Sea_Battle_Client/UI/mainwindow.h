#pragma once

#include <QMainWindow>

#include "Message.h"

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

signals:
    void sendMessageRequest(const int& to, const std::string& text);
    void searchUser(const std::string& text);
    void loadHistoryRequest(int peer_id, int last_msg_id);

private:
    Ui::MainWindow *ui;

    void refreshChat();

    void appendMessageToView(const Message& msg);//Используем для вывода 1 сообщения

    QString formatTime(int64_t timestamp);
    QString buildMessageHtml(const Message& msg);
};
