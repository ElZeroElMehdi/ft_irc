
#include "server.hpp"
#include <string>

int main(int ac, char **av)
{
    try
    {
        Server s(1337, std::string(av[1]));
        std::cout << "Server created" << std::endl;
        s.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}