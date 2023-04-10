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
        Commands(std::string _cmd, std::map<int, Clinets> &cls);
        void nick(std::string _nick, Clinets &c);
        void user(std::string _user, Clinets &c);
        void sendTo(std::string nick, std::map<int, Clinets> &cls, std::string &msg);
};
#endif