#include "client.hpp"

Clinets::Clinets(int fd):client_fd(fd)
{
    this->isRegistred = false;
}

void Clinets::setRegistred(bool _isRegistred)
{
    this->isRegistred = _isRegistred;
}
// Clinets::~Clinets()
// {
//     close(this->client_fd);
// }

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