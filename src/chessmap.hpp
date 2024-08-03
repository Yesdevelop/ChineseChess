#pragma once

#include "defines.hpp"

/// @brief 棋盘类
struct ChessMap
{
    ChessMap(CHESSMAP chessMap) : main(chessMap) {};
    CHESSMAP main;
    CHESSID &on(int x, int y);
    TEAM teamOn(int x, int y);
    array<CHESSID, 10> lineAt(int x);
};

/// @brief 访问棋盘
CHESSID &ChessMap::on(int x, int y)
{
    if (x > 8 || x < 0 || y > 9 || y < 0)
    {
        DebugHelper::error("Error when accessing chessmap: out of range");
    }
    CHESSID &v = this->main[x][y];
    return v;
}

TEAM ChessMap::teamOn(int x, int y)
{
    return toTeam(this->on(x, y));
}
/// @brief 获取一行的棋
array<CHESSID, 10> ChessMap::lineAt(int x)
{
    if (x > 8 || x < 0)
    {
        DebugHelper::error("Error when accessing chessmap: out of range");
    }
    array<CHESSID, 10> v = this->main[x];
    return v;
}
