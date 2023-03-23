#include "server.hpp"
#include <string>

void f(int)
{
    std::cout << "Signal caught" << std::endl;
    exit(0);
}
int main(int ac, char **av)
{
    try
    {
        signal(SIGINT, f);
        Server s(1337, std::string(av[1]));
        std::cout << "Server created" << std::endl;
        s.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    // s.acceptSocket();
    // s.readSocket();
    // s.writeSocket();
    return 0;
}