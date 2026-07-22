#include "../include/handler.h"

Handler::Handler(SessionManager* session_manager) : session_manager_(session_manager), dispatcher_(session_manager) {
    handlers_["chatMessage"] = [this](const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
        ChatMessage(client, j);
    };

    handlers_["ping"] = [this](const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
        Ping(client, j);
    };

    handlers_["resumeConnectionRequest"] = [this](const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
        ResumeConnectionRequest(client, j);
    };

    handlers_["createLobby"] = [this](const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
        CreateLobby(client, j);
    };

    handlers_["joinLobby"] = [this](const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
        JoinLobby(client, j);
    };

    handlers_["leaveLobby"] = [this](const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
        LeaveLobby(client, j);
    };

    handlers_["playerReadyRequest"] = [this](std::shared_ptr<ClientSession> client, const nlohmann::json& j) {
        PlayerReadyRequest(client, j);
    };
    
    handlers_["shoot"] = [this](const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
        Shoot(client, j);
    };

    handlers_["revenge"] = [this](const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
        Revenge(client, j);
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

std::string Handler::GenerateToken() {
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
    auto lobby = lobbyManager_.CreateLobby(code_, client);

    client->SetCurrentLobby(lobby);
    client->SetUsername(j["username"]);

    dispatcher_.SendTo(client, protocol::lobbyCreated(code_));
    session_manager_->Add(client);
}

void Handler::LeaveLobby(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
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

void Handler::AuthSuccess(const std::shared_ptr<ClientSession>& client, int id, const std::string& username_) {
    // client->SetUser(id, username_);
    // client->SetIsAuthentificated(true);
    // std::string token = generateTok  en();
    // userManager->createSession(id, token);
    // dispatcher_.sendTo(client, protocol::loginResponse(true, id, username_, token, ""));
    // session_manager_.add(client);
}

void Handler::ChatMessage(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    auto lobby = client->GetCurrentLobby();
    if (!lobby) {
        dispatcher_.SendTo(client, protocol::errorMessage("You are not in a lobby"));
        return;
    }

    std::string error;
    if (!Validator::valid_string_field(j, "text", Validator::message, error)) {
        dispatcher_.SendTo(client, protocol::errorMessage(error));
        return;
    }

    std::string cleanText = Validator::sanitize(j["text"].get<std::string_view>());
    if (cleanText.empty()) {
        return;
    }

    auto opponent = (lobby->GetPlayer1() == client) ? lobby->GetPlayer2() : lobby->GetPlayer1();

    if (opponent) {
        dispatcher_.SendTo(opponent, protocol::receiveChatMessage(client->GetUsername(), cleanText));
    }
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

void Handler::PlayerReadyRequest(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    auto lobby = lobbyManager_.GetPlayerLobby(client);
    if (!lobby) return;

    bool isReady = lobby->SetReady(client);
    auto game = lobby->getGame();

    if (isReady) {
        if (!game) {
            lobby->createGame();
            game = lobby->getGame();
        }

        std::vector<ShipData> ships;
        if (j.contains("ships")) {
            for (const auto& ship : j["ships"])
                ships.push_back({ship["row"], ship["column"],ship["size"], ship["horizontal"]});
        } else {
            lobby->SetReady(client);
            dispatcher_.SendTo(client, protocol::errorMessage("Not ships"));
            return;
        }

        if (!game->placeShips(client, ships)) {
            lobby->SetReady(client);
            dispatcher_.SendTo(client, protocol::errorMessage("Invalid ship placement"));
            return;
        }
    } else {
        if (game)
            game->clearShips(client);
    }

    auto opponent = lobby->GetPlayer1() == client ? lobby->GetPlayer2() : lobby->GetPlayer1();
    if (opponent) {
        dispatcher_.SendTo(opponent, protocol::playerReadyResponse(isReady));
    }

    if (lobby->IsBothReady() && game && game->getState() == GameState::PLAYING) {
        dispatcher_.SendTo(lobby->GetPlayer1(), protocol::gameStarted(game->getCurrentTurn() == lobby->GetPlayer1()));
        dispatcher_.SendTo(lobby->GetPlayer2(), protocol::gameStarted(game->getCurrentTurn() == lobby->GetPlayer2()));
    }
}

void Handler::JoinLobby(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    std::string lobby_code = j["LobbyCode"];
    std::string username_ = j["username"];
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

    if (secondPlayer)
        dispatcher_.SendTo(secondPlayer, protocol::playerJoined(username_));
    
    dispatcher_.SendTo(client, protocol::lobbyJoined(secondPlayerReady, secondPlayer->GetUsername()));
}

void Handler::NotifyPlayerLeft(const std::shared_ptr<ClientSession>& client, const std::string& playerName) {
    dispatcher_.SendTo(client, protocol::playerLeft());
}

void Handler::RemoveEmptyLobby(const std::string& code_) {
    lobbyManager_.RemoveLobby(code_);
}

void Handler::Shoot(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    auto lobby = client->GetCurrentLobby();
    if (!lobby) return;

    auto game = lobby->getGame();
    if (!game) {
        dispatcher_.SendTo(client, protocol::errorMessage("Game not found"));
        return;
    }

    int row = j["row"];
    int column = j["column"];

    ShotResult shotResult = game->makeShot(client, row, column);

    auto opponent = game->getOpponent(client);

    dispatcher_.SendTo(client, protocol::shotResult(shotResult, game->getCurrentTurn() == client));
    dispatcher_.SendTo(opponent, protocol::shotResult(shotResult, game->getCurrentTurn() == opponent));

    if (shotResult.status == ShotStatus::Win) {
        std::string winnerName = (client == game->getPlayer1()) ? game->getPlayer1()->GetUsername() : game->getPlayer2()->GetUsername();
        
        auto p1 = lobby->GetPlayer1();
        auto p2 = lobby->GetPlayer2();

        if (p1)
            dispatcher_.SendTo(p1, protocol::gameOver(winnerName, game->getOpponentShips(p1)));
        
        if (p2)
            dispatcher_.SendTo(p2, protocol::gameOver(winnerName, game->getOpponentShips(p2)));
        
        lobby->destroyGame();
    }
}

void Handler::Revenge(const std::shared_ptr<ClientSession>& client, const nlohmann::json& j) {
    auto lobby = client->GetCurrentLobby();
    if (!lobby) return;

    lobby->resetGame();
}