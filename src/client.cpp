#include "client.hpp"

Clinets::Clinets(int fd):client_fd(fd)
{
    this->setRegistred(false);
}

void Clinets::setNick(std::string nick)
{
    this->nick = nick;
    std::cout << "***> done\n";
}
void Clinets::setUser(std::string user)
{
    this->user = user;
}
void Clinets::setFd(int fd)
{
    this->client_fd = fd;
}

void Clinets::setRegistred(bool _isRegistred)
{
    this->isRegistred = _isRegistred;
}

bool Clinets::getRegistred() const
{
    return this->isRegistred;
}

Clinets::Clinets(std::string _user, std::string _nick, int _fd)
{
    if(!_user.empty())
       this->setUser(_user);
    else
        throw std::runtime_error("user is empty");
    if(!_nick.empty())
        this->setNick(_nick);
    else
        throw std::runtime_error("nick is empty");
    this->setFd(_fd);
    this->isRegistred = false;
}

Clinets::~Clinets()
{
    // chanel.clear();
    // close(this->client_fd);
}