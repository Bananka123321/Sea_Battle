#include "game_session.h"
#include "client_session.h"

GameSession::GameSession(std::shared_ptr<ClientSession> p1, std::shared_ptr<ClientSession> p2) : player1_(p1), player2_(p2), currentTurn_(p1), state_(GameState::PLACING), player1Placed_(false), player2Placed_(false) {}

bool GameSession::placeShips(std::shared_ptr<ClientSession> player, const std::vector<ShipData>& ships) {
    if (state_ != GameState::PLACING) return false;
    if (!isValidPlacement(ships)) return false;

    auto& myShips = (player == player1_) ? ships1_ : ships2_;
    myShips.clear();

    for (const auto& shipData : ships) {
        Ship ship;
        ship.row = shipData.row;
        ship.column = shipData.column;
        ship.size = shipData.size;
        ship.horizontal = shipData.horizontal;
        ship.health = shipData.size;

        for (int i = 0; i < shipData.size; ++i) {
            int r = ship.horizontal ? shipData.row : shipData.row + i;
            int c = ship.horizontal ? shipData.column + i : shipData.column;
            ship.cells.push_back({r, c});
        }
        myShips.push_back(ship);
    }

    if (player == player1_) player1Placed_ = true;
    else player2Placed_ = true;

    if (player1Placed_ && player2Placed_) {
        state_ = GameState::PLAYING;
        currentTurn_ = player1_;
    }

    return true;
}

ShotResult GameSession::makeShot(std::shared_ptr<ClientSession> player, int row, int column) {
    ShotResult result{row, column, ShotStatus::Miss, 0, false, {}};

    if (state_ != GameState::PLAYING) return result;
    if (currentTurn_ != player) return result;
    if (row < 0 || row >= 10 || column < 0 || column >= 10) return result;

    auto& opponentShips = (player == player1_) ? ships2_ : ships1_;
    
    Ship* hitShip = findShipAt(opponentShips, row, column);
    
    if (hitShip) {
        hitShip->health--;
        result.status = ShotStatus::Hit;
        result.shipSize = hitShip->size;
        result.shipHorizontal = hitShip->horizontal;

        if (hitShip->health == 0) {
            result.status = ShotStatus::Kill;
            result.shipCells = hitShip->cells;
        }
    } else {
        result.status = ShotStatus::Miss;
        currentTurn_ = getOpponent(player);
    }

    int opponentHealth = 0;
    for (const auto& ship : opponentShips) {
        opponentHealth += ship.health;
    }
    if (opponentHealth == 0) {
        state_ = GameState::FINISHED;
        result.status = ShotStatus::Win;
    }

    return result;
}

std::shared_ptr<ClientSession> GameSession::getPlayer1 () const {
    return player1_;
}

std::shared_ptr<ClientSession> GameSession::getPlayer2() const {
    return player2_;
}

std::shared_ptr<ClientSession> GameSession::getOpponent(std::shared_ptr<ClientSession> player) const {
    return (player == player1_) ? player2_ : player1_;
}

std::shared_ptr<ClientSession> GameSession::getCurrentTurn() const {
    return currentTurn_;
}

GameState GameSession::getState() const {
    return state_;
}

bool GameSession::isValidPlacement(const std::vector<ShipData>& ships) const {
    std::vector<std::vector<int>> temp(10, std::vector<int>(10, 0));
    
    for (const auto& ship : ships) {
        if (ship.horizontal && ship.column + ship.size > 10) return false;
        if (!ship.horizontal && ship.row + ship.size > 10) return false;

        for (int r = ship.row - 1; r <= ship.row + 1 + (ship.horizontal ? 0 : ship.size - 1); ++r) {
            for (int c = ship.column - 1; c <= ship.column + 1 + (ship.horizontal ? ship.size - 1 : 0); ++c) {
                if (r >= 0 && r < 10 && c >= 0 && c < 10)
                    if (temp[r][c] == 1) return false;
            }
        }

        for (int i = 0; i < ship.size; ++i) {
            int r = ship.horizontal ? ship.row : ship.row + i;
            int c = ship.horizontal ? ship.column + i : ship.column;
            temp[r][c] = 1;
        }
    }

    return true;
}

Ship* GameSession::findShipAt(std::vector<Ship>& ships, int row, int column) {
    for (auto& ship : ships) {
        for (auto& cell : ship.cells) {
            if (cell.first == row && cell.second == column) {
                return &ship;
            }
        }
    }
    return nullptr;
}

std::vector<std::pair<int,int>> GameSession::getSurroundingCells(const Ship& ship) {
    std::vector<std::pair<int,int>> surrounding;
    
    int minR = ship.row - 1;
    int maxR = ship.horizontal ? ship.row + 1 : ship.row + ship.size;
    int minC = ship.column - 1;
    int maxC = ship.horizontal ? ship.column + ship.size : ship.column + 1;

    for (int r = minR; r <= maxR; ++r) {
        for (int c = minC; c <= maxC; ++c) {
            if (r < 0 || r >= 10 || c < 0 || c >= 10) continue;
            
            bool isShipCell = false;
            for (auto& cell : ship.cells) {
                if (cell.first == r && cell.second == c) {
                    isShipCell = true;
                    break;
                }
            }
            if (!isShipCell) {
                surrounding.push_back({r, c});
            }
        }
    }
    return surrounding;
}

void GameSession::clearShips(std::shared_ptr<ClientSession> player) {
    auto& myShips = (player == player1_) ? ships1_ : ships2_;
    myShips.clear();
    
    if (player == player1_) player1Placed_ = false;
    else player2Placed_ = false;
    
    if (state_ == GameState::PLAYING) {
        state_ = GameState::PLACING;
    }
}