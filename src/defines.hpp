#pragma once

#include <array>
#include <iostream>
#include <cstring>
#include <vector>
#include <cassert>

#include "utils.hpp"

using namespace std;

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;
using CHESSID = int;
using CHESSMAP = array<array<CHESSID, 10>, 9>;

enum CHESSDEF
{
    EMPTY = 0,
    OUT = 8,
    KING = 1,
    GUARD = 2,
    BISHOP = 3,
    KNIGHT = 4,
    ROOK = 5,
    CANNON = 6,
    PAWN = 7
};


const CHESSID R_KING = 1;
const CHESSID R_GUARD = 2;
const CHESSID R_BISHOP = 3;
const CHESSID R_KNIGHT = 4;
const CHESSID R_ROOK = 5;
const CHESSID R_CANNON = 6;
const CHESSID R_PAWN = 7;
const CHESSID B_KING = -1;
const CHESSID B_GUARD = -2;
const CHESSID B_BISHOP = -3;
const CHESSID B_KNIGHT = -4;
const CHESSID B_ROOK = -5;
const CHESSID B_CANNON = -6;
const CHESSID B_PAWN = -7;

enum TEAM {
    RED = 1,
    NO_TEAM = 0,
    BLACK = -1
};

enum NODE_TYPE {
    NODE_MAX = 1,
    NODE_MIN = -1
};

const int MIN_NUMBER = -100000000;
const int MAX_NUMBER = 100000000;

const CHESSMAP DEFAULT_CHESSMAP{
    {{R_ROOK, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_ROOK},
     {R_KNIGHT, 0, R_CANNON, 0, 0, 0, 0, B_CANNON, 0, B_KNIGHT},
     {R_BISHOP, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_BISHOP},
     {R_GUARD, 0, 0, 0, 0, 0, 0, 0, 0, B_GUARD},
     {R_KING, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_KING},
     {R_GUARD, 0, 0, 0, 0, 0, 0, 0, 0, B_GUARD},
     {R_BISHOP, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_BISHOP},
     {R_KNIGHT, 0, R_CANNON, 0, 0, 0, 0, B_CANNON, 0, B_KNIGHT},
     {R_ROOK, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_ROOK}}};
int searchCount = 0;
enum TARGET
{
    TARGET_LEFT = 0,
    TARGET_RIGHT = 1,
    TARGET_UP = 2,
    TARGET_DOWN = 3
};

enum DIRECTION
{
    INDEX_LOW = 0,
    INDEX_UP = 1
};

/// @brief 获取chessid的chessdef
CHESSDEF toChessdef(CHESSID chessid)
{
    return CHESSDEF(abs(chessid));
}

/// @brief 获取chessdef的team
TEAM toTeam(CHESSID chessid)
{
    if (chessid == 0)
    {
        return NO_TEAM;
    }
    else
    {
        return chessid > 0 ? RED : BLACK;
    }
}

/// @brief 获取指定chessid的名称
string getName(CHESSID chessid)
{
    switch (chessid)
    {
    case R_KING:
        return "帅";
    case B_KING:
        return "将";
    case R_GUARD:
        return "士";
    case B_GUARD:
        return "仕";
    case R_BISHOP:
        return "相";
    case B_BISHOP:
        return "象";
    case R_KNIGHT:
        return "马";
    case B_KNIGHT:
        return "马";
    case R_ROOK:
        return "车";
    case B_ROOK:
        return "车";
    case R_CANNON:
        return "炮";
    case B_CANNON:
        return "炮";
    case R_PAWN:
        return "兵";
    case B_PAWN:
        return "卒";
    default:
        return "  ";
    }
}

/// @brief 位置类
struct Position
{
    Position(int x, int y) : x(x), y(y) {};
    int x;
    int y;
};

using TARGETS = vector<Position>;

/// @brief 着法结构
struct Action
{
    Action(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {};
    Action() : x1(0), y1(0), x2(0), y2(0) {}
    int x1;
    int y1;
    int x2;
    int y2;
};

using ACTIONS = vector<Action>;

/// @brief 搜索节点
struct Node
{
    Node(int alpha, int beta, bool type) : alpha(alpha), beta(beta), type(type) {};

    int alpha;
    int beta;
    bool type;

    int score = 0;           // 节点分数
    int scoreCumulation = 0; // 分数累计
    Action action{};         // 节点着法
};
