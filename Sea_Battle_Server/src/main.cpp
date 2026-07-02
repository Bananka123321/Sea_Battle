#include "Server.h"
#include "SignalHandler.h"

int main(int argc, char* argv[]) {
    SignalHandler::Setup();
    
    TCPServer server(6767);//В будущем хотел бы сделать выбор порта через аргументы консоли или с помощью конфига как у меня в мессенджере

    std::thread serverThread([&server](){
        if (!server.start())
            std::cerr << "Server failed to start\n";
    });

    //Ловит ctrl+C или любое иное непредвиденное завершение жизни сервера(на русской раскладке не работает, нужно доделать)
    //В дебагере тоже ничего не выведется, так как дебагер перехватывает эти сигналы и сам завершает работу программы
    while(!SignalHandler::isShutdownRequested())
        pause();

    std::cerr << "\nSERVER: signal shutdown received\n";
    server.stop();

    //Грациозно завершает поток с сервером
    if(serverThread.joinable())
        serverThread.join();

    return 0;
}