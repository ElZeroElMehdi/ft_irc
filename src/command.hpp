#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "server.hpp"
#include <string>
#include <iostream>
class Commands
{
    private:
        std::vector<cmd> com;
    public:
        Commands(std::string _cmd);
        // void nick(std::string _nick, Clinets &c);
        // void user(std::string _user, Clinets &c);
};
#endif