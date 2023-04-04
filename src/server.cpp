#include "server.hpp"

Server::Server(int pt, std::string password)
{
    if (pt < 1024 || pt > 65536)
        throw std::runtime_error("wrrong port number");
    this->port = pt;
    this->pw = password;
}

void Server::setPort(int pr)
{
    this->port = pr;
}

void Server::createSocket()
{
    this->fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (this->fd_server == -1)
        throw std::runtime_error(strerror(errno));
    int useval = 1;
    if (setsockopt(this->fd_server, SOL_SOCKET, SO_REUSEADDR, &useval, sizeof(useval)) == -1)
    {
        close(this->fd_server);
        throw std::runtime_error("faild to set socket options socket");
    }
    if (fcntl(this->fd_server, F_SETFL, O_NONBLOCK) == -1)
    {
        close(this->fd_server);
        throw std::runtime_error("faild to set change mode of socket");
    }
}
void Server::bindSocket()
{
    this->ConAddr.sin_family = AF_INET;
    this->ConAddr.sin_addr.s_addr = INADDR_ANY;
    this->ConAddr.sin_port = htons(this->port);

    if (bind(this->fd_server, (struct sockaddr *)&this->ConAddr, sizeof(this->ConAddr)) != 0)
    {
        close(this->fd_server);
        throw std::runtime_error("bind faild");
    }
}

void Server::listenConix()
{
    if (listen(this->fd_server, -1) == -1)
    {
        close(this->fd_server);
        throw std::runtime_error(strerror(errno));
    }
}

void Server::addFd(int fd)
{
    struct pollfd newFd;
    newFd.fd = fd;
    newFd.events = POLLIN;
    this->allFd.push_back(newFd);
}

bool Server::events()
{
    if (poll(&this->allFd[0], this->allFd.size(), -1) == -1)
    {
        close(this->fd_server);
        return false;
    }
    // request?
    if (this->allFd.at(0).revents & POLLIN)
    {
        if (this->allFd.at(0).revents & POLLIN)
        {
            int len = sizeof(ConAddr);
            int newClient = accept(this->fd_server, (struct sockaddr *)&ConAddr, (socklen_t *)&len);
            if (newClient == -1)
            {
                close(this->fd_server);
                return false;
            }
            this->addFd(newClient);
            std::cout << "one client connectd\n";
            send(newClient, "welcome to my server\n", 21, 0);
        }
    }
    return true;
}

void Server::creatServer()
{
    this->createSocket();
    this->bindSocket();
    this->listenConix();
    this->addFd(this->fd_server);
    // poll event
    while (1)
    {
        if (!this->events())
            break;
        this->chat();
    }
}

void Server::chat()
{
    for (size_t i = 0; i < this->allFd.size(); i++)
    {
        if (this->allFd.at(i).revents & POLLIN && (this->allFd.at(i).fd != this->fd_server))
        {
            char msg[1024];
            memset(msg, 0, 1024);
            recv(this->allFd[i].fd, msg, 1024, 0);
            std::cout << "client"<<this->allFd[i].fd<<" : "<< msg;
        }
    }
}

Server::~Server()
{
}