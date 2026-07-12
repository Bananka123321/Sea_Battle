#include "server.h"

TCPServer::TCPServer(int port_) : port_(port_), server_socket_(-1), session_manager_(), handler_(session_manager_) {}

TCPServer::~TCPServer() {
    Stop();
}

bool TCPServer::Start() {
    if (!SetupSocket()) return false;

    StartClientMonitoring();
    Run();

    return true;
}

void TCPServer::Stop() {
    if(monitorRunning_.load()) monitorRunning_.store(false);
    if(serverRunning_.load()) serverRunning_.store(false);
    if(monitor_thread_.joinable()) monitor_thread_.join();
    
    if (server_socket_ != -1) {
        shutdown(server_socket_, SHUT_RDWR);
        close(server_socket_);
    }
}

//Создание сервера прослушивателя 
bool TCPServer::SetupSocket() {
    server_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket_ == -1) {
        std::cerr << "Failed to create socket_\n";
        return false;
    }

    //Функция убирающая проблемы с быстрым перезапуском сервера(я не помню в чем там была проблема)
    //В общем сервер просто как-то долго закрывается и зачищает порты с какими-то
    //проверками, и эта функция просто в лоб закрывает всё
    int opt = 1;
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) { //Forced sestart server
        std::cerr << "setsockopt failed\n";
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Bind failed\n";
        return false;
    }

    if (listen(server_socket_, SOMAXCONN) == -1) { 
        std::cerr << "Listen failed\n";
        return false;
    }

    std::cout << "Server listening on port_ " << port_ << std::endl;
    return true;
}

//Отвечает за подключение новых клиентов к серверу
void TCPServer::Run() {
    serverRunning_ = true;
    sockaddr_in clientAddr{};
    socklen_t addrLen = sizeof(clientAddr); 
    while (serverRunning_.load()) {
        int clientSocket = accept(server_socket_, (sockaddr*)&clientAddr, (socklen_t*)&addrLen);
        if (clientSocket == -1) continue;

        auto client = std::make_shared<ClientSession>(clientSocket);
        
        std::thread(&TCPServer::HandleClient, this, client).detach();
    }
    std::cerr << "!!!SERVER STOPED!!!\n";
}


//Каждый клиент - 1 поток, соответственно тут каждый поток отвечает за прослушивание 1 клиента
void TCPServer::HandleClient(std::shared_ptr<ClientSession> client) {
    try {
        std::string msg;
        client->SetConnected(true);
        while(client->Receive(msg))
            handler_.HandleMessage(client, msg);
        ClientDisconnect(client);
    } catch(std::exception& e){
        std::cerr << "Client thread ERROR: " << e.what() << '\n';
        ClientDisconnect(client);
    }
}

void TCPServer::ClientDisconnect(const std::shared_ptr<ClientSession>& client) {
    if(!client->GetConnected()) return;

    client->SetConnected(false);
    
    auto lobby = client->GetCurrentLobby();
    if (lobby) {
        auto secondPlayer = lobby->GetPlayer1() == client ? lobby->GetPlayer2() : lobby->GetPlayer1();
        if (secondPlayer)
            handler_.NotifyPlayerLeft(secondPlayer, client->GetUsername());
        
        lobby->RemovePlayer(client);
        client->SetCurrentLobby(nullptr);
        
        if (!lobby->GetPlayer1() && !lobby->GetPlayer2())
            handler_.RemoveEmptyLobby(lobby->GetCode());
    }
    
    shutdown(client->GetSocket(), SHUT_RDWR);
    session_manager_->Remove(client);
    close(client->GetSocket());
}


//В будущем будет отвечать за проверку соединения с каждый клиентом в каждом лобби
void TCPServer::StartClientMonitoring() {
    monitorRunning_ = true;
    monitor_thread_ = std::thread([this](){
        while (monitorRunning_.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(HEARTBEAT_INTERVAL_MS));

            int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

            auto clients = session_manager_->GetAll();

            for (const auto& client : clients) {
                if(now - client->GetLastActivity() > SESSION_TIMEOUT_MS) {
                    ClientDisconnect(client);
                }
            }
        }
    });

    monitor_thread_.detach();
}