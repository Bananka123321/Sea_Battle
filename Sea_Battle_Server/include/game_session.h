#pragma once

#include <memory>
#include <vector>

#include "client_session.h"
#include "ship_coord.h"

enum class GameState {
    PLACING,
    PLAYING,
    FINISHED
};

enum class CellState {
    Empty = 0,
    Ship = 1,
    Miss = 2,
    Hit = 3,
    Kill = 4
};

class GameSession {
public:
    GameSession(std::shared_ptr<ClientSession> p1, std::shared_ptr<ClientSession> p2);

    bool placeShips(std::shared_ptr<ClientSession> player, const std::vector<ShipData>& ships);
    
    int makeShot(std::shared_ptr<ClientSession> player, int row, int col);

    std::shared_ptr<ClientSession> getPlayer1() const;
    std::shared_ptr<ClientSession> getPlayer2() const;
    std::shared_ptr<ClientSession> getOpponent(std::shared_ptr<ClientSession> player) const;
    std::shared_ptr<ClientSession> getCurrentTurn() const;
    GameState getState() const;

private:
    std::shared_ptr<ClientSession> player1_;
    std::shared_ptr<ClientSession> player2_;
    std::shared_ptr<ClientSession> currentTurn_;

    std::vector<std::vector<CellState>> board1_;
    std::vector<std::vector<CellState>> board2_;

    std::vector<std::vector<CellState>> shots1_;
    std::vector<std::vector<CellState>> shots2_;

    GameState state_;
    bool player1Placed_;
    bool player2Placed_;

    bool isValidPlacement(const std::vector<ShipData>& ships) const;
    int getShipsHealth(int playerIndex) const;
};