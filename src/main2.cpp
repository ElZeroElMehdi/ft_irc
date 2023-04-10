#include "command.hpp"

int main()
{
    Commands cmd("prvmsg nick1,nick2,nick3 :hello\r\njoin #channel");
    return 0;
}