#include "Handler.h"

Handler::Handler() {
    handlers["chatMessage"] = [this] (const nlohmann::json& j) {
        onMessage(j["from"], j["text"]);
    };

    handlers["error"] = [this] (const nlohmann::json& j) {
        onError(j["message"]);
    };

    handlers["resumeConnectionResponse"] = [this] (const nlohmann::json& j) {
        onConnectionResponse(j["success"]);
    };

    handlers["lobbyCreated"] = [this] (const nlohmann::json& j) {
        onLobbyCreated(j["lobbyCode"]);
    };

    handlers["lobbyJoined"] = [this] (const nlohmann::json& j) {
        onLobbyJoined(j["success"], j["username"]);
    };

    handlers["playerJoined"] = [this] (const nlohmann::json& j) {
        onPlayerJoined(j["username"]);
    };

    handlers["playerLeft"] = [this] (const nlohmann::json& j) {
        onPlayerLeft();
    };

    handlers["playerReadyResponse"] = [this] (const nlohmann::json& j) {
        onPlayerReadyResponse(j["success"]);
    };

    handlers["gameStarted"] = [this] (const nlohmann::json& j) {
        onGameStarted(j["yourTurn"]);
    };

    handlers["shotResult"] = [this] (const nlohmann::json& j) {
        int row = j["row"];
        int column = j["column"];
        int status = j["status"];
        bool yourTurn = j["yourTurn"];

        bool shipSunk = (status == 2);
        std::vector<std::pair<int, int>> shipCells;

        if (shipSunk && j.contains("shipCells") && j["shipCells"].is_array()) {
            for (const auto& cell : j["shipCells"]) {
                shipCells.push_back({cell["row"], cell["column"]});
            }
        }

        onShotResult(row, column, status, yourTurn, shipSunk, shipCells);
    };

    handlers["gameOver"] = [this] (const nlohmann::json& j) {
        onGameOver(j["winner"]);
    };
}


///Функция парсит пришедший пакет и вызывает соответствующую лямбда функцию из
///заранее готового списка handlers заполненного в конструкторе выше
void Handler::handleMessage(const std::string& msg) {
    try {
        auto j = nlohmann::json::parse(msg);
        std::string type = j["type"];//Получает тип пакета

        if (handlers.find(type) != handlers.end())
            handlers[type](j);//если такой имеется вызывает соответствующую функцию
        else
            std::cerr << "Unknown message type " << type << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "JSON parse ERROR " << e.what() << std::endl;
    }
}

//=============================================================================================

void Handler::onMessage(const std::string& from, const std::string& text) {
    emit S_Message(from, text);
}

void Handler::onError(const std::string& text) {
    std::cerr << text << '\n';
}

void Handler::onConnectionResponse(const bool& success) {
    if(success)
        emit S_ConnectionSucess();
}

void Handler::onLobbyCreated(const std::string& code) {
    emit S_LobbyCreated(code);
}

void Handler::onLobbyJoined(bool success, const std::string& username) {
    emit S_LobbyJoined(success, username);
}

void Handler::onPlayerJoined(const std::string& username) {
    emit S_PlayerJoined(username);
}

void Handler::onPlayerLeft() {
    emit S_PlayerLeft();
}

void Handler::onPlayerReadyResponse(bool success) {
    emit S_EnemyReady(success);
}

void Handler::onGameStarted(bool yourTurn) {
    emit S_GameStarted(yourTurn);
}

void Handler::onShotResult(int row, int column, int status, bool yourTurn, bool shipSunk, const std::vector<std::pair<int, int>>& shipCells) {
    emit S_ShotResult(row, column, status, yourTurn, shipSunk, shipCells);
}

void Handler::onGameOver(const std::string& winner) {
    emit S_onGameOver(winner);
}
