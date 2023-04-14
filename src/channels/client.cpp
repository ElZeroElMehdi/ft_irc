#include "client.hpp"

Clients::Clients(int fd) : client_fd(fd)
{
    this->setRegistred(false);
    this->nick = "";
    this->user = "";
}

void Clients::setNick(std::string nick)
{
    this->nick = nick;
}
void Clients::setUser(std::string user)
{
    this->user = user;
}
void Clients::setFd(int fd)
{
    this->client_fd = fd;
}

void Clients::setRegistred(bool _isRegistred)
{
    this->isRegistred = _isRegistred;
}

bool Clients::getRegistred() const
{
    return this->isRegistred;
}

std::string Clients::getNick() const
{
    return this->nick;
}

Clients::Clients(std::string _user, std::string _nick, int _fd)
{
    if (!_user.empty())
        this->setUser(_user);
    else
        throw std::runtime_error("user is empty");
    if (!_nick.empty())
        this->setNick(_nick);
    else
        throw std::runtime_error("nick is empty");
    this->setFd(_fd);
    this->isRegistred = false;
}

int Clients::getFd() const
{
    return this->client_fd;
}

std::string Clients::getUser() const{
    return this->user;
}

Clients::~Clients()
{
}

bool Clients::checkIfRegistred()
{
    if (!this->nick.empty() && !this->user.empty())
    {
        this->isRegistred = true;
        return true;
    }
    else
    {
        this->isRegistred = false;
        return false;
    }
}