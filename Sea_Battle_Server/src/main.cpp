#include "server.h"
#include "signal_handler.h"

int main(int argc, char* argv[]) {
    SignalHandler::Setup();
    
    TCPServer server(7070);//В будущем хотел бы сделать выбор порта через аргументы консоли или с помощью конфига как у меня в мессенджере

    std::thread serverThread([&server](){
        if (!server.Start())
            std::cerr << "Server failed to start\n";
    });

    //Ловит ctrl+C или любое иное непредвиденное завершение жизни сервера(на русской раскладке не работает, нужно доделать)
    //В дебагере тоже ничего не выведется, так как дебагер перехватывает эти сигналы и сам завершает работу программы
    while(!SignalHandler::IsShutdownRequested())
        pause();

    std::cerr << "\nSERVER: signal shutdown received\n";
    server.Stop();

    //Грациозно завершает поток с сервером
    if(serverThread.joinable())
        serverThread.join();

    return 0;
}