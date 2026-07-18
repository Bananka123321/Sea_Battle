#include "game_session.h"
#include "client_session.h"

GameSession::GameSession(std::shared_ptr<ClientSession> p1, std::shared_ptr<ClientSession> p2) : player1_(p1), player2_(p2), currentTurn_(p1), state_(GameState::PLACING), player1Placed_(false), player2Placed_(false) {
    board1_ = board2_ = std::vector<std::vector<CellState>>(10, std::vector<CellState>(10, CellState::Empty));
    shots1_ = shots2_ = std::vector<std::vector<CellState>>(10, std::vector<CellState>(10, CellState::Empty));
}

bool GameSession::placeShips(std::shared_ptr<ClientSession> player, const std::vector<ShipData>& ships) {
    if (state_ != GameState::PLACING) return false;
    if (!isValidPlacement(ships)) return false;

    auto& board = (player == player1_) ? board1_ : board2_;
    
    board = std::vector<std::vector<CellState>>(10, std::vector<CellState>(10, CellState::Empty));
    
    for (const auto& ship : ships) {
        for (int i = 0; i < ship.size; ++i) {
            int r = ship.horizontal ? ship.row : ship.row + i;
            int c = ship.horizontal ? ship.column + i : ship.column;
            board[r][c] = CellState::Ship;
        }
    }

    if (player == player1_) player1Placed_ = true;
    else player2Placed_ = true;

    if (player1Placed_ && player2Placed_) {
        state_ = GameState::PLAYING;
        currentTurn_ = player1_;
    }

    return true;
}

int GameSession::makeShot(std::shared_ptr<ClientSession> player, int row, int col) {
    if (state_ != GameState::PLAYING) return -1;
    if (currentTurn_ != player) return -1;
    if (row < 0 || row >= 10 || col < 0 || col >= 10) return -1;

    auto& opponentBoard = (player == player1_) ? board2_ : board1_;
    auto& myShots = (player == player1_) ? shots1_ : shots2_;

    if (myShots[row][col] != CellState::Empty) return -1;

    int result = 0;
    
    if (opponentBoard[row][col] == CellState::Ship) {
        result = 1;
        myShots[row][col] = CellState::Hit;
    } else {
        result = 0;
        myShots[row][col] = CellState::Miss;
        currentTurn_ = getOpponent(player);
    }

    int opponentIndex = (player == player1_) ? 2 : 1;
    if (getShipsHealth(opponentIndex) == 0) {
        state_ = GameState::FINISHED;
        return 2;
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

int GameSession::getShipsHealth(int playerIndex) const {
    const auto& board = (playerIndex == 1) ? board1_ : board2_;
    int health = 0;
    for (int r = 0; r < 10; ++r) {
        for (int c = 0; c < 10; ++c) {
            if (board[r][c] == CellState::Ship)
                health++;
        }
    }

    return health;
}

void GameSession::clearShips(std::shared_ptr<ClientSession> player) {
    auto& board = (player == player1_) ? board1_ : board2_;
    board = std::vector<std::vector<CellState>>(10, std::vector<CellState>(10, CellState::Empty));
    
    if (player == player1_) player1Placed_ = false;
    else player2Placed_ = false;
    
    if (state_ == GameState::PLAYING) {
        state_ = GameState::PLACING;
    }
}