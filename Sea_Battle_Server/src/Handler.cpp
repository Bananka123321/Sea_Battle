#include "../include/Handler.h"

Handler::Handler(SessionManager& sm) : sessionManager(sm), dispatcher(sessionManager) {
    handlers["loginRequest"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        loginRequest(client, j);
    };

    handlers["registerRequest"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        registerRequest(client, j);
    };

    handlers["privateMessage"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        privateMessage(client, j);
    };

    handlers["searchUserRequest"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        searchUserRequest(client, j);
    };

    handlers["ping"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        ping(client, j);
    };

    handlers["resumeConnectionRequest"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        resumeConnectionRequest(client, j);
    };

    handlers["createLobby"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        createLobby(client, j);
    };

    handlers["joinLobby"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        joinLobby(client, j);
    };

    handlers["leaveLobby"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        leaveLobby(client, j);
    };

    handlers["readyLobby"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        readyLobby(client, j);
    };
}

Handler::~Handler() {}

void Handler::handleMessage(std::shared_ptr<ClientSession> client, std::string& msg) {
    try {
        auto j = nlohmann::json::parse(msg);
        sessionManager.updateActivity(client);
        std::string type = j["type"];
        if (handlers.find(type) != handlers.end()) {
            handlers[type](client, j);
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



void Handler::loginRequest(std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
    // std::string error;
    // if(!Validator::valid_string_field(j, "username", Validator::username, error) || !Validator::valid_string_field(j, "password", Validator::password, error)) {
    //     dispatcher.sendTo(client, protocol::loginResponse(false, -1, "", error));
    //     return;
    // }

    // auto res = userManager.loginUser(j["username"], j["password"]);
    // if (!res.success) {
    //     dispatcher.sendTo(client, protocol::loginResponse(res.success, res.user_id, j["username"], "", res.error));
    //     return;
    // }

    // authSuccess(client, res.user_id, j["username"]);
}

void Handler::registerRequest(std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
    // std::string error;
    // if(!Validator::valid_string_field(j, "username", Validator::username, error) || !Validator::valid_string_field(j, "password", Validator::password, error)) {
    //     dispatcher.sendTo(client, protocol::registerResponse(false, -1, "", error));
    //     return;
    // }
    
    // auto res = userManager.registerUser(j["username"],j["password"]);
    // if (!res.success) {
    //     dispatcher.sendTo(client, protocol::loginResponse(res.success, res.user_id, j["username"], "", res.error));
    //     return;
    // }

    // authSuccess(client, res.user_id, j["username"]);
}

void Handler::authSuccess(std::shared_ptr<ClientSession> client, const int& id, const std::string& username) {
    // client->setUser(id, username);
    // client->setIsAuthentificated(true);
    // std::string token = generateToken();
    // userManager.createSession(id, token);
    // dispatcher.sendTo(client, protocol::loginResponse(true, id, username, token, ""));
    // sessionManager.add(client);
}

void Handler::privateMessage(std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
    if (!client->getIsAuthentificated()) {
        std::cerr << "SORRY, not auth\n";
        return;
    }
    
    std::string error;
    if(!Validator::valid_string_field(j, "text", Validator::message, error)) {
        dispatcher.sendTo(client, protocol::errorMessage(error));
        return;
    }

    if(!Validator::valid_int_field(j, "to", 1, std::numeric_limits<long int>::max(), error)) {
        dispatcher.sendTo(client, protocol::errorMessage(error));
        return;
    }

    std::string cleanText = Validator::sanitize(j["text"].get<std::string_view>());
    if (cleanText.empty()) {
        dispatcher.sendTo(client, protocol::errorMessage("Message is empty after sanitization"));
        return;
    }

    int64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    auto receiver = sessionManager.getByUserId(j["to"]);
    if (!receiver) return;

    nlohmann::json out = j;
    out["text"] = cleanText;
    receiver->send(out.dump());
}

void Handler::setDisconnectHandler(std::function<void(std::shared_ptr<ClientSession>)> cb) {
    dispatcher.onDisconnect = cb;
}

void Handler::searchUserRequest(std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
    // std::string error;
    // if(!Validator::valid_string_field(j, "username", Validator::search, error)) {
    //     dispatcher.sendTo(client, protocol::errorMessage(error));
    //     return;
    // }

    // auto users = userManager.searchUsers(j["username"]);

    // dispatcher.sendTo(client, protocol::searchUserResponse(users));
}

void Handler::ping(std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
    sessionManager.updateActivity(client);
}

void Handler::resumeConnectionRequest(std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
    // std::string token = j.value("token", "");
    // if (token.empty()) {
    //     std::cerr << "token empty\n";
    //     dispatcher.sendTo(client, protocol::resumeConnectionResponse(false));
    //     return;
    // }

    // auto user_id = userManager.getUserIdByToken(token);
    // if(!user_id.has_value()) {
    //     dispatcher.sendTo(client, protocol::resumeConnectionResponse(false));
    //     return;
    // }

    // int id = user_id.value();
    // client->setUser(id, "hz");
    // client->setIsAuthentificated(true);
    // sessionManager.add(client);
    // dispatcher.sendTo(client, protocol::resumeConnectionResponse(true));
}

void Handler::createLobby(std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
    std::string code = lobbyManager.generateLobbyCode();
    std::cerr << "LobbyCode: " << code << '\n';
    auto lobby = lobbyManager.createLobby(code, client);

    client->setCurrentLobby(lobby);

    dispatcher.sendTo(client, protocol::lobbyCreated(code));
    sessionManager.add(client);
}

void Handler::leaveLobby(std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
    auto lobby = lobbyManager.getPlayerLobby(client);
    if(!lobby) return;

    auto secondPlayer = lobby->getPlayer1() == client ? lobby->getPlayer2() : lobby->getPlayer1();
    if(secondPlayer)
        dispatcher.sendTo(secondPlayer, protocol::playerLeft());
    
    lobbyManager.removePlayerFromLobby(client);
    client->setCurrentLobby(nullptr);
}

void Handler::readyLobby(std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
    auto lobby = lobbyManager.getPlayerLobby(client);
    if(!lobby) return;

    lobby->setReady(client);

    if(lobby->isBothReady()) {
        dispatcher.sendTo(lobby->getPlayer1(), protocol::readyLobby());
        dispatcher.sendTo(lobby->getPlayer2(), protocol::readyLobby());
    }
    // else {
    //     auto secondPlayer = lobby->getPlayer1() == client ? lobby->getPlayer2() : lobby->getPlayer1();
    //     if(!secondPlayer) return;
    //     dispatcher.sendTo(secondPlayer, protocol::);
    // } //Пока хз, можно потом наверное добавить как уведомление второму игроку о готовности первого


}

void Handler::joinLobby(std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
    std::string lobby_code = j["LobbyCode"];
    std::string player_name = j["username"];
    
    auto lobby = lobbyManager.getLobby(lobby_code);
    
    if (!lobby) {
        dispatcher.sendTo(client, protocol::errorMessage("Lobby not found"));
        return;
    }
    
    if (lobby->isFull()) {
        dispatcher.sendTo(client, protocol::errorMessage("Lobby is full"));
        return;
    }
    
    lobby->addPlayer(client);
    client->setCurrentLobby(lobby);
    
    dispatcher.sendTo(lobby->getPlayer1(), protocol::playerJoined(client->getUsername()));
    
    dispatcher.sendTo(client, protocol::lobbyJoined());
}