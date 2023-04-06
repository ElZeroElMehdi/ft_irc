#include "channels.hpp"

Channels::Channels(std::string name, int admin)
{
    this->name = name;
    this->admin = admin;
}

void Channels::addUser(int fd)
{
    this->users.push_back(fd);
}

void Channels::removeUser(int fd)
{
    std::vector<int>::iterator it = std::find(this->users.begin(), this->users.end(), fd);
    if (it != this->users.end())
        this->users.erase(it);
}

std::string Channels::getName() const
{
    return this->name;
}

int Channels::getAdmin() const
{
    return this->admin;
}

Channels::~Channels()
{
}