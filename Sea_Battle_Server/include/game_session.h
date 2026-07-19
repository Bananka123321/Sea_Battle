#pragma once

#include <memory>
#include <vector>

#include "ship_coord.h"

class ClientSession;

enum class GameState {
    PLACING,
    PLAYING,
    FINISHED
};

struct Ship {
    int row;
    int column;
    int size;
    bool horizontal;
    int health;
    std::vector<std::pair<int,int>> cells;
};

class GameSession {
public:
    GameSession(std::shared_ptr<ClientSession> p1, std::shared_ptr<ClientSession> p2);

    bool placeShips(std::shared_ptr<ClientSession> player, const std::vector<ShipData>& ships);
    
    ShotResult makeShot(std::shared_ptr<ClientSession> player, int row, int col);

    std::shared_ptr<ClientSession> getPlayer1() const;
    std::shared_ptr<ClientSession> getPlayer2() const;
    std::shared_ptr<ClientSession> getOpponent(std::shared_ptr<ClientSession> player) const;
    std::shared_ptr<ClientSession> getCurrentTurn() const;
    GameState getState() const;

    void clearShips(std::shared_ptr<ClientSession> player);

private:
    std::shared_ptr<ClientSession> player1_;
    std::shared_ptr<ClientSession> player2_;
    std::shared_ptr<ClientSession> currentTurn_;

    std::vector<Ship> ships1_;
    std::vector<Ship> ships2_;

    GameState state_;
    bool player1Placed_;
    bool player2Placed_;

    bool isValidPlacement(const std::vector<ShipData>& ships) const;
    Ship* findShipAt(std::vector<Ship>& ships, int row, int column);
    std::vector<std::pair<int,int>> getSurroundingCells(const Ship& ship);
};