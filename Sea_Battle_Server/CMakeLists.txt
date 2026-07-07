cmake_minimum_required(VERSION 3.15)
project(BattleshipServer)

set(SERVER_SOURCES
    src/main.cpp
    src/Server.cpp
    src/ClientSession.cpp
    src/SessionManager.cpp
    src/Handler.cpp
    src/MessageDispatcher.cpp
    src/SignalHandler.cpp
    src/LobbyManager.cpp
    src/lobby.cpp

    ../common/Validator.cpp
    ../common/PacketIO.cpp
)

add_executable(Server ${SERVER_SOURCES})

target_include_directories(Server PRIVATE
    include/
    ../common/
)