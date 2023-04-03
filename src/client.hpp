#ifndef CLIENT_HPP
# define CLINET_HPP

#include "includes.hpp"

class Clinets
{
    private:
        int client_fd;
    public:
        Clinets(int fd);
};

Clinets::Clinets(int fd):client_fd(fd)
{
}


#endif