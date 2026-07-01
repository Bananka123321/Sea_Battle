#include "Server.h"

TCPServer::TCPServer(int port, Handler handler) : port(port), serverSocket(-1), handler_(handler) {}

TCPServer::~TCPServer() {
    stop();
}

bool TCPServer::start() {
    if (!setupSocket()) return false;

    startClientMonitoring();
    run();

    return true;
}

void TCPServer::stop() {
    if(monitorRunning.load()) monitorRunning.store(false);
    if(serverRunning.load()) serverRunning.store(false);
    if(monitor_thread.joinable()) monitor_thread.join();
    
    if (serverSocket != -1) {
        shutdown(serverSocket, SHUT_RDWR);
        close(serverSocket);
    }
}

bool TCPServer::setupSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket\n";
        return false;
    }

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) { //Forced sestart server
        std::cerr << "setsockopt failed\n";
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Bind failed\n";
        return false;
    }

    if (listen(serverSocket, SOMAXCONN) == -1) { 
        std::cerr << "Listen failed\n";
        return false;
    }

    std::cout << "Server listening on port " << port << std::endl;
    return true;
}

//Отвечает за подключение новых клиентов к серверу
void TCPServer::run() {
    serverRunning = true;
    sockaddr_in clientAddr{};
    socklen_t addrLen = sizeof(clientAddr); 
    while (serverRunning.load()) {
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, (socklen_t*)&addrLen);
        if (clientSocket == -1) continue;

        auto client = std::make_shared<ClientSession>(clientSocket);
        
        std::thread(&TCPServer::handleClient, this, client).detach();
    }
    std::cerr << "!!!SERVER STOPED!!!\n";
}


//Каждый клиент - 1 поток, соответственно тут каждый поток отвечает за прослушивание 1 клиента
void TCPServer::handleClient(std::shared_ptr<ClientSession> client) {
    try {
        std::string msg;
        client->setConnected(true);
        while(serverRunning.load() && client->receive(msg))
            handler_.handleMessage(client, msg);
        clientDisconnect(client);
    } catch(std::exception& e){
        std::cerr << "Client thread ERROR: " << e.what() << '\n';
        clientDisconnect(client);
    }
}

void TCPServer::clientDisconnect(std::shared_ptr<ClientSession> client) {
    if(!client->getConnected()) return;

    client->setConnected(false);

    shutdown(client->getSocket(), SHUT_RDWR);

    sessionManager_.remove(client);

    close(client->getSocket());
}


//В будущем будет отвечать за проверку соединения с каждый клиентом в каждом лобби
void TCPServer::startClientMonitoring() {
    monitorRunning = true;
    monitor_thread = std::thread([this](){
        while (monitorRunning.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(HEARTBEAT_INTERVAL_MS));

            int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

            auto clients = sessionManager_.getAll();

            for (const auto& client : clients) {
                if(now - client->getLastActivity() > SESSION_TIMEOUT_MS) {
                    clientDisconnect(client);
                }
            }
        }
    });

    monitor_thread.detach();
}