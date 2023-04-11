#ifndef INCLUDES_HPP
# define INCLUDES_HPP 

#include <iostream>

#include <sys/socket.h>
// #include <sys/types.h>  //not required in lunix
#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <vector>
#include <map>
#include <string.h>

struct cmd
{
    std::string command;// command
    std::vector<std::string> target;// users or channels
    std::string msg;//msg or empty // :fvcz
};

// #include "channels.hpp"
// #include "client.hpp"
// #include "server.hpp"

#endif