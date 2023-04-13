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
    for (size_t i = 0; i < this->users.size(); i++)
    {
        if (this->users[i] == fd)
        {
            this->users.erase(this->users.begin() + i);
            break;
        }
    }
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