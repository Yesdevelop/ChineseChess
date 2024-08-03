#include "board.hpp"

int main()
{
    Board board {};
    board.printBoard();

    time_t beginTime = DebugHelper::getTime();
    board.makeDecision(4);
    int duration = DebugHelper::duration(beginTime);
    cout << "迭代了" << searchCount << "次" << endl;
    cout << "平均节点耗时：" << float(duration) / float(searchCount) << endl;
    board.printBoard();

    system("pause");
    return 0;
}
