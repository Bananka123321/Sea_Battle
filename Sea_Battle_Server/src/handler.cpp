#include "../include/handler.h"

Handler::Handler(SessionManager* session_manager) : session_manager_(session_manager), dispatcher_(session_manager) {
    handlers_["PrivateMessage"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        PrivateMessage(client, j);
    };

    handlers_["Ping"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        Ping(client, j);
    };

    handlers_["ResumeConnectionRequest"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        ResumeConnectionRequest(client, j);
    };

    handlers_["CreateLobby"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        CreateLobby(client, j);
    };

    handlers_["joinLobby"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        joinLobby(client, j);
    };

    handlers_["leaveLobby"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        leaveLobby(client, j);
    };

    handlers_["playerReadyRequest"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        playerReadyRequest(client, j);
    };
}

Handler::~Handler() {}

void Handler::HandleMessage(const std::shared_ptr<ClientSession>& client, std::string& message) {
    try {
        auto j = nlohmann::json::parse(message);
        session_manager_->UpdateActivity(client);
        std::string type = j["type"];
        if (handlers_.find(type) != handlers_.end()) {
            handlers_[type](client, j);
        }
        else
            std::cerr << "Unknown message type " << type << std::endl;    

    } catch (const std::exception& e) {
        std::cerr << "JSON parse ERROR " << e.what() << std::endl;
    }
}

static std::mt19937& getGlobalRNG() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

std::string Handler::generateToken() {
    static std::mutex rng_mutex;
    std::lock_guard<std::mutex> lock(rng_mutex);

    auto& gen = getGlobalRNG();
    std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);
    std::stringstream ss;
    for(int i = 0; i < 4; i++)
        ss << std::hex << std::setw(8) << std::setfill('0') << dis(gen);
    return ss.str();
}



//============================================================================================================================

void Handler::CreateLobby(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    std::string code_ = lobbyManager_.GenerateLobbyCode();
    std::cerr << "LobbyCode: " << code_ << '\n';
    auto lobby = lobbyManager_.CreateLobby(code_, client);

    client->SetCurrentLobby(lobby);
    client->SetUsername(j["username_"]);

    dispatcher_.SendTo(client, protocol::lobbyCreated(code_));
    session_manager_->Add(client);
}

void Handler::leaveLobby(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    auto lobby = lobbyManager_.GetPlayerLobby(client);
    if(!lobby) return;

    auto secondPlayer = lobby->GetPlayer1() == client ? lobby->GetPlayer2() : lobby->GetPlayer1();
    if(secondPlayer)
        dispatcher_.SendTo(secondPlayer, protocol::playerLeft());
    
    lobby->RemovePlayer(client);
    client->SetCurrentLobby(nullptr);
    
    if (!lobby->GetPlayer1() && !lobby->GetPlayer2())
        lobbyManager_.RemoveLobby(lobby->GetCode());
}

void Handler::authSuccess(const std::shared_ptr<ClientSession>& client, int id, const std::string& username_) {
    // client->SetUser(id, username_);
    // client->SetIsAuthentificated(true);
    // std::string token = generateToken();
    // userManager->createSession(id, token);
    // dispatcher_.sendTo(client, protocol::loginResponse(true, id, username_, token, ""));
    // session_manager_.add(client);
}

void Handler::PrivateMessage(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    if (!client->GetIsAuthentificated()) {
        std::cerr << "SORRY, not auth\n";
        return;
    }
    
    std::string error;
    if(!Validator::valid_string_field(j, "text", Validator::message, error)) {
        dispatcher_.SendTo(client, protocol::errorMessage(error));
        return;
    }

    if(!Validator::valid_int_field(j, "to", 1, std::numeric_limits<long int>::max(), error)) {
        dispatcher_.SendTo(client, protocol::errorMessage(error));
        return;
    }

    std::string cleanText = Validator::sanitize(j["text"].get<std::string_view>());
    if (cleanText.empty()) {
        dispatcher_.SendTo(client, protocol::errorMessage("Message is empty after sanitization"));
        return;
    }

    int64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    auto receiver = session_manager_->GetByUserId(j["to"]);
    if (!receiver) return;

    nlohmann::json out = j;
    out["text"] = cleanText;
    receiver->Send(out.dump());
}

void Handler::SetDisconnectHandler(DisconnectCallback callback) {
    dispatcher_.SetDisconnectCallback(callback);
}

void Handler::Ping(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    session_manager_->UpdateActivity(client);
}

void Handler::ResumeConnectionRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    // std::string token = j["token"];
    // if (token.empty()) {
    //     std::cerr << "token empty\n";
    //     dispatcher_.SendTo(client, protocol::resumeConnectionResponse(false));
    //     return;
    // }

    // auto user_id_ = userManager.getUserIdByToken(token);
    // if(!user_id_.has_value()) {
    //     dispatcher_.sendTo(client, protocol::resumeConnectionResponse(false));
    //     return;
    // }

    // int id = user_id_.value();
    // client->SetUser(id, "hz");
    // client->SetIsAuthentificated(true);
    // session_manager_.add(client);
    // dispatcher_.sendTo(client, protocol::resumeConnectionResponse(true));
}

void Handler::playerReadyRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    auto lobby = lobbyManager_.GetPlayerLobby(client);
    if(!lobby) return;

    auto secondPlayer = lobby->GetPlayer1() == client ? lobby->GetPlayer2() : lobby->GetPlayer1();
    dispatcher_.SendTo(secondPlayer, protocol::playerReadyResponse(lobby->SetReady(client)));

    if(lobby->IsBothReady()) {
        dispatcher_.SendTo(lobby->GetPlayer1(), protocol::lobbyReady());
        dispatcher_.SendTo(lobby->GetPlayer2(), protocol::lobbyReady());
    }
}

void Handler::joinLobby(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    std::string lobby_code = j["LobbyCode"];
    std::string username_ = j["username_"];
    client->SetUsername(username_);

    auto lobby = lobbyManager_.GetLobby(lobby_code);
    
    if (!lobby) {
        dispatcher_.SendTo(client, protocol::errorMessage("Lobby not found"));
        return;
    }
    
    if (lobby->IsFull()) {
        dispatcher_.SendTo(client, protocol::errorMessage("Lobby is full"));
        return;
    }
    
    lobby->AddPlayer(client);
    client->SetCurrentLobby(lobby);
    
    auto secondPlayer = lobby->GetPlayer1() == client ? lobby->GetPlayer2() : lobby->GetPlayer1();
    bool secondPlayerReady = lobby->GetPlayer1() == client ? lobby->GetPlayer2Ready() : lobby->GetPlayer1Ready();

    dispatcher_.SendTo(secondPlayer, protocol::playerJoined(username_));
    
    dispatcher_.SendTo(client, protocol::lobbyJoined(secondPlayerReady, secondPlayer->GetUsername()));
}

void Handler::NotifyPlayerLeft(const std::shared_ptr<ClientSession>& client, const std::string& playerName) {
    dispatcher_.SendTo(client, protocol::playerLeft());
}

void Handler::RemoveEmptyLobby(const std::string& code_) {
    lobbyManager_.RemoveLobby(code_);
}