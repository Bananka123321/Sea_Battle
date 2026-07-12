#pragma once

#include <json.hpp>
#include <string>
#include <vector>
#include "Message.h"

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

inline std::string privateMessage(const int& sender, const int& user, const std::string& text) {
    nlohmann::json j;
    j["type"] = "privateMessage";
    j["text"] = text;
    j["from"] = sender;
    j["to"] = user;
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

inline std::string playerReadyRequest() {
    nlohmann::json j;
    j["type"] = "playerReadyRequest";
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

}
