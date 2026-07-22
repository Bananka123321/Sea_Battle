#pragma once

#include <json.hpp>
#include <string>
#include <vector>

#include "Message.h"
#include "ship_coord.h"

//======================== Message ===================================

inline void to_json(nlohmann::json& j, const Message& m) {
    j = {
        {"message_id", m.msgId},
        {"receiver_id", m.receiverId},
        {"sender_id", m.senderId},
        {"text", m.text},
        {"timestamp", m.timestamp}
    };
}

inline void from_json(const nlohmann::json& j, Message& m) {
    j.at("message_id").get_to(m.msgId);
    j.at("receiver_id").get_to(m.receiverId);
    j.at("sender_id").get_to(m.senderId);
    j.at("text").get_to(m.text);
    j.at("timestamp").get_to(m.timestamp);
}


namespace protocol {

    
//      CHAT
//=================================================================================================================================================================

inline std::string chatMessage(const std::string& text) {
    nlohmann::json j;
    j["type"] = "chatMessage";
    j["text"] = text;
    return j.dump();
}

inline std::string receiveChatMessage(const std::string& from, const std::string& text) {
    nlohmann::json j;
    j["type"] = "chatMessage";
    j["from"] = from;
    j["text"] = text;
    return j.dump();
}

//      CLIENT --> SERVER
//=================================================================================================================================================================

inline std::string ping() {
    nlohmann::json j;
    j["type"] = "ping";
    return j.dump();
}

inline std::string resumeConnectionRequest(const std::string& token) {
    nlohmann::json j;
    j["type"] = "resumeConnectionRequest";
    j["token"] = token;
    return j.dump();
}

inline std::string createLobby(const std::string& username) {
    nlohmann::json j;
    j["type"] = "createLobby";
    j["username"] = username;
    return j.dump();
}

inline std::string joinLobby(const std::string& username, const std::string& LobbyCode) {
    nlohmann::json j;
    j["type"] = "joinLobby";
    j["username"] = username;
    j["LobbyCode"] = LobbyCode;
    return j.dump();
}

inline std::string leaveLobby() {
    nlohmann::json j;
    j["type"] = "leaveLobby";
    return j.dump();
}

inline std::string playerReadyRequest(const std::vector<ShipData>& ships) {
    nlohmann::json j;
    j["type"] = "playerReadyRequest";

    nlohmann::json shipsArray = nlohmann::json::array();
    for (const auto& ship : ships) {
        shipsArray.push_back({
            {"row", ship.row},
            {"column", ship.column},
            {"size", ship.size},
            {"horizontal", ship.horizontal}
        });
    }
    
    j["ships"] = shipsArray;

    return j.dump();
}

inline std::string shoot(int row, int column) {
    nlohmann::json j;
    j["type"] = "shoot";
    j["row"] = row;
    j["column"] = column;
    return j.dump();
}

inline std::string revenge() {
    nlohmann::json j;
    j["type"] = "revenge";
    return j.dump();
}

//      SERVER --> CLIENT
//=================================================================================================================================================================


inline std::string errorMessage(const std::string& reason) {
    nlohmann::json j;
    j["type"] = "error";
    j["message"] = reason;
    return j.dump();
}

inline std::string resumeConnectionResponse(bool success) {
    nlohmann::json j;
    j["type"] = "resumeConnectionResponse";
    j["success"] = success;
    return j.dump();
}

inline std::string lobbyCreated(const std::string& lobbyCode) {
    nlohmann::json j;
    j["type"] = "lobbyCreated";
    j["lobbyCode"] = lobbyCode;
    return j.dump();
}

inline std::string lobbyJoined(bool success, const std::string& username = "") {//Для подключившегося игрока
    nlohmann::json j;
    j["type"] = "lobbyJoined";
    j["success"] = success;
    j["username"] = username;
    return j.dump();
}

inline std::string playerJoined(const std::string& username) {//Уведомление для хоста
    nlohmann::json j;
    j["type"] = "playerJoined";
    j["username"] = username;
    return j.dump();
}

inline std::string playerLeft() {
    nlohmann::json j;
    j["type"] = "playerLeft";
    return j.dump();
}

inline std::string playerReadyResponse(bool success) {
    nlohmann::json j;
    j["type"] = "playerReadyResponse";
    j["success"] = success;
    return j.dump();
}

inline std::string lobbyReady() {
    nlohmann::json j;
    j["type"] = "lobbyReady";
    return j.dump();
}


inline std::string gameStarted(bool yourTurn) {
    nlohmann::json j;
    j["type"] = "gameStarted";
    j["yourTurn"] = yourTurn;
    return j.dump();
}

inline std::string shotResult(const ShotResult& result, bool yourTurn) {
    nlohmann::json j;
    j["type"] = "shotResult";
    j["row"] = result.row;
    j["column"] = result.column;
    j["status"] = static_cast<int>(result.status);
    j["yourTurn"] = yourTurn;
    
    if (result.status == ShotStatus::Kill) {
        j["shipSize"] = result.shipSize;
        j["shipHorizontal"] = result.shipHorizontal;
        
        nlohmann::json cellsArray = nlohmann::json::array();
        for (const auto& cell : result.shipCells) {
            cellsArray.push_back({{"row", cell.first}, {"column", cell.second}});
        }
        j["shipCells"] = cellsArray;
    }
    
    return j.dump();
}

inline std::string gameOver(const std::string& winner, const std::vector<ShipData>& opponentShips) {
    nlohmann::json j;
    j["type"] = "gameOver";
    j["winner"] = winner;
    
    nlohmann::json shipsArray = nlohmann::json::array();
    for (const auto& ship : opponentShips) {
        shipsArray.push_back({
            {"row", ship.row},
            {"column", ship.column},
            {"size", ship.size},
            {"horizontal", ship.horizontal}
        });
    }
    j["opponentShips"] = shipsArray;
    
    return j.dump();
}

};
