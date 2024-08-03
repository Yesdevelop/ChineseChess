#pragma once

#include "defines.hpp"

/// @brief 位棋盘
class BitBoard
{
public:
    /// @brief 初始化位棋盘
    BitBoard()
    {
        initPublicResource();
        clearBitBoard();
    }

    /// @brief 用其它棋盘初始化位棋盘
    explicit BitBoard(const int board[256])
    {
        initPublicResource();
        readFromBoard(board);
    }

    /// @brief 初始化资源
    void initPublicResource()
    {
        // 初始化判断棋子间障碍的屏蔽位
        for (int i = 3; i < 13; i++)
        {
            for (int a = 3; a < 13; a++)
            {
                for (int from = min(i, a) + 1; from < max(i, a); from++)
                {
                    mayExistBarrier[i][a] |= ((uint16)1 << from);
                }
            }
        }
        // 初始化检测车炮攻击目标的数组
        memset(RayBits, -1, sizeof(int) * 1024 * 10 * 2 * 4);
        for (uint16 bits = 0; bits < 1024; bits++)
        {
            for (int from = 0; from < 10; from++)
            {
                // low -> 下标减小的一侧
                const int low_index = 0;
                for (int to = from - 1, cnt = 0; to >= 0 && cnt < 4; to--)
                {
                    if (bits & ((uint16)1 << to))
                    {
                        RayBits[bits][from][low_index][cnt] = to;
                        cnt++;
                    }
                }
                // up -> 下标增加的一侧
                const int up_index = 1;
                for (int to = from + 1, cnt = 0; to < 10 && cnt < 4; to++)
                {
                    if (bits & ((uint16)1 << to))
                    {
                        RayBits[bits][from][up_index][cnt] = to;
                        cnt++;
                    }
                }
            }
        }
    }

    /// @brief 棋盘转位棋盘
    void readFromBoard(const int board[256])
    {
        clearBitBoard();
        // 通过外部数组，初始化位棋盘
        for (int pos = 51; pos < 205; pos++)
        {
            if (board[pos])
            {
                yBits[getY(pos)] |= ((uint16)1 << (getX(pos)));
                xBits[getX(pos)] |= ((uint16)1 << (getY(pos)));
            }
        }
    }

    /// @brief 清空位棋盘
    void clearBitBoard()
    {
        // 清空数组
        memset(yBits, (uint16)0, 16 * sizeof(uint16));
        memset(xBits, (uint16)0, 16 * sizeof(uint16));
    }

    /// @brief 打印位棋盘
    /// @param printTranspose
    void printBitBoard(bool printTranspose = true)
    {
        // 打印整个棋盘的bit位
        cout << endl;
        cout << "xBits:" << endl;
        for (unsigned short xBit : xBits)
        {
            printBitLine(xBit);
        }
        // 打印转置棋盘的bit位
        if (printTranspose)
        {
            cout << "yBits:" << endl;
            for (unsigned short yBit : yBits)
            {
                printBitLine(yBit);
            }
        }
    }

    /// @brief 打印
    static void printBitLine(uint16 Bits)
    {
        // 打印单行的bit位
        for (int i = 0; i < 16; i++)
        {
            if (Bits & ((uint16)1 << i))
            {
                cout << 1;
            }
            else
            {
                cout << ".";
            }
            if (i != 15)
            {
                cout << " ";
            }
        }
        cout << endl;
    }

    /// @brief 获取一行内的障碍物
    /// @param yFrom
    /// @param xFrom
    /// @param yTo
    /// @param xTo
    /// @return
    bool checkLineExistBarrier(int yFrom, int xFrom, int yTo, int xTo)
    {
        assert((xFrom != xTo) || (yFrom != yTo));
        // 检查啷个棋子中间是否有障碍物
        if (xFrom != xTo)
        {
            return mayExistBarrier[xFrom][xTo] & yBits[yFrom];
        }
        return mayExistBarrier[yFrom][yTo] & xBits[xFrom];
    }

    /// @brief 步进
    /// @param yFrom
    /// @param xFrom
    /// @param yTo
    /// @param xTo
    void makeMove(int yFrom, int xFrom, int yTo, int xTo)
    {
        // 步进
        yBits[yFrom] &= ~((uint16)1 << xFrom);
        yBits[yTo] |= ((uint16)1 << xTo);
        xBits[xFrom] &= ~((uint16)1 << yFrom);
        xBits[xTo] |= ((uint16)1 << yTo);
    }

    /// @brief 撤销步进
    void unMakeMove(int yFrom, int xFrom, int yTo, int xTo, bool eaten)
    {
        // 撤销步进
        yBits[yFrom] |= ((uint16)1 << xFrom);
        xBits[xFrom] |= ((uint16)1 << yFrom);
        if (!eaten)
        {
            yBits[yTo] &= ~((uint16)1 << xTo);
            xBits[xTo] &= ~((uint16)1 << yTo);
        }
    }

    /// @brief 获取一条射线上的目标
    /// @param pos
    /// @param target
    /// @param num
    /// @return
    int getRayTarget(const int pos, const int target, int num)
    {
        assert(num >= 0 && num < 4);
        // pos 棋子位置 | target 攻击方向 | num 第几个棋子(从0开始数)
        const int x = getX(pos);
        const int y = getY(pos);
        //-2 错误值 | -1 没有被攻击到的棋子 | 攻击到的棋子索引(需要±3和存储格式对齐)
        int rayTarget = -2;
        switch (target)
        {
        case TARGET_LEFT:
            rayTarget = RayBits[yBits[y] >> 3][x - 3][INDEX_LOW][num];
            break;
        case TARGET_RIGHT:
            rayTarget = RayBits[yBits[y] >> 3][x - 3][INDEX_UP][num];
            break;
        case TARGET_UP:
            rayTarget = RayBits[xBits[x] >> 3][y - 3][INDEX_LOW][num];
            break;
        case TARGET_DOWN:
            rayTarget = RayBits[xBits[x] >> 3][y - 3][INDEX_UP][num];
            break;
        default:
            cout << "error occurred in the func named getRayTarget()" << endl;
            break;
        }
        assert(rayTarget != -2);
        if (rayTarget != -1)
        {
            return rayTarget + 3;
        }
        return rayTarget;
    }

protected:
    /// @brief 获得棋子的列索引
    static int getX(int pos)
    {
        return pos & 15;
    }

    /// @brief 获得棋子的行索引
    static int getY(int pos)
    {
        return (pos >> 4);
    }
    /// @brief 用于检测车炮，即“射线”类棋子的攻击目标 | low : 0 & up : 1
    int RayBits[1024][10][2][4];
    /// @brief 用于检测棋子之间的障碍物
    uint16 mayExistBarrier[16][16];

private:
    uint16 yBits[16]{}; // 横向是位向量
    uint16 xBits[16]{}; // 纵向是位向量

    friend class position;
};
