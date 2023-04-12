#include "client.hpp"

Clinets::Clinets(int fd) : client_fd(fd)
{
    this->setRegistred(false);
    this->nick = "";
    this->user = "";
}

void Clinets::setNick(std::string nick)
{
    this->nick = nick;
}
void Clinets::setUser(std::string user)
{
    this->user = user;
}

void Clinets::setSecendUser(std::string user)
{
    this->SecendUser = user;
}

std::string Clinets::getSecendUser() const
{
    return this->SecendUser;
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

std::string Clinets::getNick() const
{
    return this->nick;
}

Clinets::Clinets(std::string _user, std::string _nick, int _fd)
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

int Clinets::getFd() const
{
    return this->client_fd;
}

std::string Clinets::getUser() const{
    return this->user;
}

Clinets::~Clinets()
{
    // chanel.clear();
    // close(this->client_fd);
}

bool Clinets::checkIfRegistred()
{
    if (!this->nick.empty() && !this->user.empty())
    {
        this->isRegistred = true;
        std::cout << "client registred\n";
        return true;
    }
    else
    {
        this->isRegistred = false;
        return false;
    }
}