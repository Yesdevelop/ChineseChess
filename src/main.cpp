#include "board.hpp"

int main()
{
    Board board {};
    board.printBoard();
    board.makeDecision(3);
    cout << searchCount << endl;
    board.printBoard();
    system("pause");
    return 0;
}
