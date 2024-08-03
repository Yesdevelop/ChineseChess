#pragma once

#include <ctime>

using namespace std;

/// @brief 日志工具类
class DebugHelper
{
public:
    /// @brief 报错并暂停代码
    /// @param msg 错误信息
    static void error(string msg = "An Error Occured")
    {
        cerr << msg << endl;
        system("pause");
    }

    /// @brief 启动一个时间节点
    /// @returns 毫秒时间戳
    static clock_t getTime()
    {
        return clock();
    }

    /// @brief 结束并打印耗时
    /// @param beginTime
    static int duration(clock_t beginTime)
    {
        int ms = clock() - beginTime;
        clog << "耗时：" << ms << "ms" << endl;
        return ms;
    }
};

/// @brief 获取数组（或其他可迭代容器如vector）中最大值索引
template <typename T>
int getIndexOfMax(T array)
{
    int maxIndex = 0;
    int maxNumber = array.at(0);

    for (int i = 0; i < array.size(); i++)
    {
        int number = array.at(i);
        if (number > maxNumber)
        {
            maxNumber = number;
            maxIndex = i;
        }
    }

    return maxIndex;
}

/// @brief 获取数组（或其他可迭代容器如vector）中最小值索引
template <typename T>
int getIndexOfMin(T array)
{
    int minIndex = 0;
    int minNumber = array.at(0);

    for (int i = 0; i < array.size(); i++)
    {
        int number = array.at(i);
        if (number < minNumber)
        {
            minNumber = number;
            minIndex = i;
        }
    }

    return minIndex;
}
