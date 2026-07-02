#include "MessageRouter.h"
#include "tcp_client.h"
#include "Handler.h"
#include "appcontroller.h"
#include "StateBinder.h"

#include <QApplication>
#include <QMetaObject>
#include "QFile"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    AppState* state = new AppState();
    MessageRouter* router = new MessageRouter();
    Handler* handler = new Handler();
    TCPClient* client = new TCPClient(6767, router);
    AppController* AController = new AppController(router, state, handler, client);
    StateChanger* binder = new StateChanger(handler, state);

    client->onMessage = [handler](const std::string& msg) {
        QMetaObject::invokeMethod(handler, [handler, msg]() {
            handler->handleMessage(msg);
        }, Qt::QueuedConnection);
    };

    std::thread([client]() { client->start(); }).detach();

    a.exec();
    delete client;
    delete handler;
    delete state;
    return 0;
}
