#pragma once

#include <QObject>
#include <QTimer>

#include "MessageRouter.h"
#include "mainwindow.h"
#include "tcp_client.h"
#include "Handler.h"

static int64_t PING_TIME_MS = 30000;//Время пингования сервера
static int32_t MAX_RECONNECT_TIME_MS = 30000;//Максимальный промежуток между удачными пингами

class AppController : public QObject {
    Q_OBJECT

public:
    AppController(MessageRouter* router, Handler* handler, TCPClient* client);
    ~AppController();

    void AttachUI(MainWindow* mainW);
    void startPing();
    void stopPing();
    void startReconnect();

signals:
    void ping();

private:
    TCPClient* client_;
    Handler* handler_;
    MessageRouter* router_;
    QTimer* pingTimer = nullptr;
    QTimer* reconnectTimer = nullptr;

    int reconnectAttempts = 0;
};
