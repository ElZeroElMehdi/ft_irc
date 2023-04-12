#include "includes.hpp"
#include "server.hpp"
int main(int ac, char **argv)
{
    try
    {
        if (ac != 3 || !argv[1] || !argv[2])
            throw std::runtime_error("invlaide arguments");
        Server s(argv[1], argv[2]);
        s.creatServer();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}