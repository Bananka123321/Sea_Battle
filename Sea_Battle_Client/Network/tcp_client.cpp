#include "tcp_client.h"
#include <QString>

QString IPADRESS_dev = "127.0.0.1";//Айпи для локальной разработки
QString IPADRESS_dep = "31.192.108.37";//Айпи реального сервера в новосибе

TCPClient::TCPClient(int port, MessageRouter* msgRouter) : port_(port), clientSocket(-1), router_(msgRouter) {}

TCPClient::~TCPClient() {
    if (clientSocket != -1) {
#ifdef _WIN32//Закрытие сокета для винды
        closesocket(clientSocket);
#else//Закрытие сокета для остальных ОС
        close(clientSocket);
#endif
    }
}

bool TCPClient::start() {
#ifdef _WIN32//Что-то нужное для винды
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return false;
    }
#endif

    if(work.joinable()) work.join();
    if(clientSocket != -1) {
#ifdef _WIN32
        closesocket(clientSocket);
#else
        close(clientSocket);
#endif
        clientSocket = -1;
    }

    if (!setupSocket()) return false;

    work = std::thread([this](){
        runLoop();
    });

    return true;
}

bool TCPClient::setupSocket() {
    clientSocket = socket(AF_INET, SOCK_STREAM ,IPPROTO_TCP);
    if (clientSocket == -1) {
        return false;
    }

    int flag = 1;
    setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_);

    if (inet_pton(AF_INET, IPADRESS_dep.toUtf8().constData(), &serverAddr.sin_addr) <= 0) {
        return false;
    }

    if (::connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
#ifdef _WIN32
        closesocket(clientSocket);
#else
        close(clientSocket);
#endif
        clientSocket = -1;
        return false;
    }

    handoverSocket();

    return true;
}

void TCPClient::handoverSocket() {
    router_->setSocket(clientSocket);
}

void TCPClient::runLoop() {
    bConnected.store(true);

    QMetaObject::invokeMethod(this, [this](){
        emit connected();
    });

    std::string msg;
    while (PacketIO::recvPacket(clientSocket, msg))
        onMessage(msg);//Эта функция реализована в main как лямбда функция вызывающая функцию в handler

    QMetaObject::invokeMethod(this, [this](){
        emit connectionLose();
    });
}

void TCPClient::disconnect() {
    if(!bConnected.load()) return;

    if (clientSocket != -1) {
#ifdef _WIN32
        closesocket(clientSocket);
#else
        close(clientSocket);
#endif
    }
    if(work.joinable()) work.join();
}
