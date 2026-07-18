cmake_minimum_required(VERSION 3.5)
project(Sea_Battle_Client)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Test)

set(PROJECT_SOURCES
    main.cpp
    Network/tcp_client.cpp
    Network/MessageRouter.cpp

    App/AppState.cpp
    App/Handler.cpp
    App/appcontroller.cpp
    App/shipitem.cpp
    App/board.cpp
    App/GameManager.cpp
    App/LobbyManager.cpp
    App/gameboard.cpp
    App/gameshipitem.cpp
    App/clickablescene.cpp
    App/graphicsboard.cpp

    UI/mainwindow.cpp

    ../common/PacketIO.cpp
    ../common/Validator.cpp
)

set(PROJECT_HEADERS
    Network/tcp_client.h
    Network/MessageRouter.h

    App/AppState.h
    App/Handler.h
    App/appcontroller.h
    App/shipitem.h
    App/board.h
    App/GameManager.h
    App/LobbyManager.h
    App/gameboard.h
    App/gameshipitem.h
    App/clickablescene.h
    App/graphicsboard.h

    UI/mainwindow.h

    ../common/PacketIO.h
    ../common/Message.h
    ../common/protocol.h
    ../common/Validator.h
)

qt_add_executable(Sea_Battle_Client
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}

    UI/mainwindow.ui
    resources.qrc
)

target_include_directories(Sea_Battle_Client PRIVATE
    Network
    App
    UI
    ../common
)

target_link_libraries(Sea_Battle_Client PRIVATE
    Qt6::Widgets
)

if (WIN32)
    target_link_libraries(Sea_Battle_Client PRIVATE ws2_32)
endif()
