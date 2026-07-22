#pragma once

#include <QObject>
#include <QTimer>

#include "MessageRouter.h"
#include "mainwindow.h"
#include "tcp_client.h"
#include "Handler.h"

class AppController : public QObject {
    Q_OBJECT

public:
    AppController(MessageRouter* router, Handler* handler, TCPClient* client);
    ~AppController();

    void AttachUI(MainWindow* mainW);

signals:

private:
    TCPClient* client_;
    Handler* handler_;
    MessageRouter* router_;
};
