#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "includes.hpp"

class Channels
{
    private:
        std::string name;
        int admin;
        std::vector<int> users;
    public:
        Channels(std::string name, int admin);
        void addUser(int fd);
        void removeUser(int fd);
        std::string getName() const;
        int getAdmin() const;
        ~Channels();
};

#endif