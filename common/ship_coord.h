#pragma once

struct ShipData {
    int row;
    int column;
    int size;
    bool horizontal;
};

enum class ShotStatus {
    Miss = 0,
    Hit = 1,
    Kill = 2,
    Win = 3
};

struct ShotResult {
    int row;
    int column;
    ShotStatus status;
    int shipSize = 0;
    bool shipHorizontal = false;
    std::vector<std::pair<int,int>> shipCells;
};