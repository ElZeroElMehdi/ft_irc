#include "client.hpp"

Clients::Clients(int fd) : client_fd(fd), status(false), isRegistred(false)
{
    this->setRegistred(false);
    this->nick = "";
    this->user = "";
}

void Clients::setNick(std::string nick)
{
    this->nick = nick;
}
void Clients::SetStatus()
{
    status = 1;
}
void Clients::setUser(std::string user)
{
    this->user = user;
}
std::string Clients::getPass() const
{
    return this->pass;
}

void Clients::setSecendUser(std::string user)
{
    this->SecendUser = user;
}

std::string Clients::getSecendUser() const
{
    return this->SecendUser;
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

Clients::Clients(std::string _user, std::string _nick, int _fd):status(0)
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
    // chanel.clear();
    // close(this->client_fd);
}

bool Clients::checkIfRegistred()
{
    if (!this->nick.empty() && !this->user.empty() && !this->pass.empty() && !this->isRegistred)
        this->isRegistred = true;
    else
        return false;
    return true;
}

void Clients::setIp(std::string ip)
{
    this->ip = ip;
}

std::string Clients::getIp() const
{
    return this->ip;
}

void Clients::setPort(unsigned int port)
{
    this->port = port;
}

unsigned int Clients::getPort() const
{
    return this->port;
}

bool Clients::getStatus() const
{
    return this->status;
}

void Clients::setPass(std::string pass)
{
    this->pass = pass;
}

void Clients::setHostName(std::string hostName)
{
    this->hostName = hostName + ".ip";
}

std::string Clients::getHostName(std::string server_host) const
{
    return nick + "!~" + user + "@" + server_host;
}
