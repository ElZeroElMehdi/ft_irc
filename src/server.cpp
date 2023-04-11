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

    // why???
    Clinets newClient(fd);
    this->cl.insert(std::make_pair(fd, newClient));
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
            std::string s = ":localhost NOTICE AUTH :*** Looking up your hostname...\n:localhost NOTICE AUTH :*** Found your hostname\n";
            
            send(newClient, s.c_str(), s.length(), 0);
            // send(newClient, s.c_str(), s.length(), 0);
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
    while (1)
    {
        if (this->events())
            this->chat();
        // break;
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
            if (this->cl.find(this->allFd[i].fd)->second.getRegistred() == true)
            {
                recv(this->allFd[i].fd, msg, 1024, 0);
                std::string cmd = msg;
                std::cout << cmd << std::endl;
                if (cmd.substr(0, 4) == "QUIT" || cmd.substr(0, 4) == "quit")
                {
                    std::string s = "bye bye\n";
                    if (this->cl.find(this->allFd[i].fd)->second.getRegistred() == true)
                        std::cout << this->cl.find(this->allFd[i].fd)->second.getNick() << " leave\n";
                    send(this->allFd[i].fd, s.c_str(), s.length(), 0);
                    close(this->allFd[i].fd);
                    this->allFd.erase(this->allFd.begin() + i);
                    this->cl.erase(this->allFd[i].fd);
                    break;
                }
            }
            else
            {
                recv(this->allFd[i].fd, msg, 1024, 0);
                std::string msgg = msg;
                if (msgg.substr(0, 4) == "NICK" || msgg.substr(0, 4) == "nick")
                    this->cl.find(this->allFd[i].fd)->second.setNick(msgg.substr(5, msgg.length() - 6));
                if (msgg.substr(0, 4) == "user" || msgg.substr(0, 4) == "USER")
                    this->cl.find(this->allFd[i].fd)->second.setUser(msgg.substr(5, msgg.length() - 6));
                if (this->cl.find(this->allFd[i].fd)->second.checkIfRegistred())
                {
                    char ip[INET_ADDRSTRLEN];
                    std::string s = generateMessage(this->cl.find(this->allFd[i].fd)->second.getNick(), inet_ntop(AF_INET, &this->ConAddr.sin_addr, ip, sizeof(ip)), "localhost");
                    send(this->allFd[i].fd, s.c_str(), s.length(), 0);
                }
            }
        }
        if (this->cl.find(this->allFd[i].fd)->second.getRegistred() == false && this->allFd.at(i).revents & POLLHUP)
        {
            std::cout << "Client" << this->allFd[i].fd << " disconnected" << std::endl;
            close(this->allFd[i].fd);
            this->allFd.erase(this->allFd.begin() + i);
            this->cl.erase(this->allFd[i].fd);
        }
        else if (this->allFd.at(i).revents & POLLHUP)
        {
            std::cout << this->cl.find(this->allFd[i].fd)->second.getNick();
            std::cout << " disconnected" << std::endl;
            close(this->allFd[i].fd);
            this->allFd.erase(this->allFd.begin() + i);
            this->cl.erase(this->allFd[i].fd);
        }
    }
}

int Server::findClinet(std::string nickName)
{
    for (std::map<int, Clinets>::iterator it = this->cl.begin(); it != this->cl.end(); it++)
    {
        if (it->second.getNick() == nickName)
            return it->first;
    }
    return -1;
}

Server::~Server()
{
    close(this->fd_server);
}

/*
            GRAMMER
<command> ::= <message> | <join-command> | <part-command> | <quit-command> | <nick-command>

<message> ::= PRIVMSG <recipient> <text> //
<recipient> ::= <channel> | <user>

<join-command> ::= JOIN <channel>
<channel> ::= "#" <channel-name>
<channel-name> ::= <letter> { <letter> | <digit> | "-" }

<part-command> ::= PART <channel>

<quit-command> ::= QUIT [ <message> ]

<nick-command> ::= NICK <nickname>
<nickname> ::= <letter> { <letter> | <digit> | "-" | "_" }

<text> ::= <any printable character except CR or LF> { <any printable character except CR or LF> }

*/
