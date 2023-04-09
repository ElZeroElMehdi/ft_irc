#include "command.hpp"

int main()
{
    Commands cmd("NICK nick\r\nUSER user\r\nprvmsg nick1 :hello");
    return 0;
}