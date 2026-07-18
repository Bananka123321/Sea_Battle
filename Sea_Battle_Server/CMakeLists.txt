cmake_minimum_required(VERSION 3.15)
project(BattleshipServer)

set(SERVER_SOURCES
    src/main.cpp
    src/server.cpp
    src/client_session.cpp
    src/session_manager.cpp
    src/handler.cpp
    src/message_dispatcher.cpp
    src/signal_handler.cpp
    src/lobby_manager.cpp
    src/lobby.cpp
    src/game_session.cpp

    ../common/Validator.cpp
    ../common/PacketIO.cpp
)

add_executable(Server ${SERVER_SOURCES})

target_include_directories(Server PRIVATE
    include/
    ../common/
)