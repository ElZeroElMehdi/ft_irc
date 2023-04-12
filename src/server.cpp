#include "server.hpp"
#include "commands.hpp"

Server::Server(const char *pt, const char *password)
{
    int por = std::atoi(pt);
    if (por < 1024 || por > 65536)
        throw std::runtime_error("wrrong port number!");
    this->port = por;
    this->pw = password;
}

void Server::setPort(int pr)
{
    this->port = pr;
}

int Server::getPort() const
{
    return this->port;
}

void Server::createSocket()
{
    this->fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (this->fd_server == -1)
        throw std::runtime_error("creating socket faild!"); // strerror(errno)
    int useval = 1;
    if (setsockopt(this->fd_server, SOL_SOCKET, SO_REUSEADDR, &useval, sizeof(useval)) == -1)
    {
        close(this->fd_server);
        throw std::runtime_error("faild to set socket options socket!");
    }
    if (fcntl(this->fd_server, F_SETFL, O_NONBLOCK) == -1)
    {
        close(this->fd_server);
        throw std::runtime_error("faild to set change mode of socket!");
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
        throw std::runtime_error("bind faild!");
    }
}

void Server::listenConix()
{
    if (listen(this->fd_server, -1) == -1)
    {
        close(this->fd_server);
        throw std::runtime_error("no connection comming!"); // strerror(errno)
    }
}

void Server::addFd(int fd)
{
    struct pollfd newFd;
    newFd.fd = fd;
    newFd.events = POLLIN;
    this->allFd.push_back(newFd);

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
    // check if a requist is comming
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
        // std::string s = ":localhost NOTICE AUTH :*** Looking up your hostname...\n:localhost NOTICE AUTH :*** Found your hostname\n";
        // send(newClient, s.c_str(), s.length(), 0);//send a msg to the client as reply to the connection request
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
                std::cout << "*****> " << cmd << std::endl;
                // Commands(cmd, this->cl.find(this->allFd[i].fd)->second);
                if (cmd.substr(0, 4) == "NICK" || cmd.substr(0, 4) == "nick")
                {
                    this->cl.find(this->allFd[i].fd)->second.setNick(cmd.substr(5, cmd.length() - 6));
                    std::string s = ":" + this->cl.find(this->allFd[i].fd)->second.getNick() + "!" + this->cl.find(this->allFd[i].fd)->second.getNick() + "@" + this->cl.find(this->allFd[i].fd)->second.getNick() + " NICK :" + this->cl.find(this->allFd[i].fd)->second.getNick() + "\n";
                    send(this->allFd[i].fd, s.c_str(), s.length(), 0);
                }
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
                std::cout << msgg << std::endl;
                Commands(msgg,this->allFd[i].fd,  this->cl);
                // if (msgg.substr(0, 4) == "NICK" || msgg.substr(0, 4) == "nick")
                //     this->cl.find(this->allFd[i].fd)->second.setNick(msgg.substr(5, msgg.length() - 6));
                // if (msgg.substr(0, 4) == "USER" || msgg.substr(0, 4) == "user")
                // {
                //     this->cl.find(this->allFd[i].fd)->second.setUser(msgg.substr(5, msgg.length() - 6));
                //     this->cl.find(this->allFd[i].fd)->second.setSecendUser(msgg.substr(5, msgg.length() - 6));
                //     std::cout << "*****> " << msgg.substr(5, msgg.length() - 6) << std::endl;
                // }
                // if (this->cl.find(this->allFd[i].fd)->second.checkIfRegistred())
                // {
                //     for (size_t j = 0; j < 5; j++)
                //     {
                //         std::string s = this->showReply(j + 1, this->allFd[i].fd);
                //         send(this->allFd[i].fd, s.c_str(), s.length(), 0);
                //     }
                //     std::string lastWelcomeReply = ":" + this->cl.find(this->allFd[i].fd)->second.getNick() + " MODE " + this->cl.find(this->allFd[i].fd)->second.getNick() + " :+iH\n";
                //     send(this->allFd[i].fd, lastWelcomeReply.c_str(), lastWelcomeReply.length(), 0);
                // }
            }
        }
        if (this->cl.find(this->allFd[i].fd)->second.getRegistred() == false && this->allFd.at(i).revents & POLLHUP)
        {
            std::cout << "Client" << this->allFd[i].fd << " disconnected*" << std::endl;
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

std::string Server::showReply(int code, int fd)
{
    char ipv[INET_ADDRSTRLEN];
    std::string ip = inet_ntop(AF_INET, &this->ConAddr.sin_addr, ipv, sizeof(ipv));
    std::string Nick = this->cl.find(fd)->second.getNick();
    std::string User = this->cl.find(fd)->second.getUser();

    std::string s;
    std::vector<std::string> str;
    if (code == 1)
    {
        s = Nick + "," + User + "," + ip;
        str = splitString(s, ",");
        s = get_replay(code, str).msg;
        // s = ":" + ip + " " + s;
        str.clear();
    }
    else if (code == 2)
    {
        s = ip + ", 2.0";
        str = splitString(s, ",");
        s = get_replay(code, str).msg;
        str.clear();
    }
    else if (code == 3)
    {
        str.push_back("2023-01-01");
        s = get_replay(code, str).msg;
        str.clear();
    }
    else if (code == 4)
    {
        str.push_back(ip);
        str.push_back("2.0");
        str.push_back("H+");
        str.push_back("M+");
        s = get_replay(code, str).msg;
        str.clear();
    }
    else if (code == 5)
    {
        str.push_back(ip);
        str.push_back(ft_itoa(this->getPort()));
        s = get_replay(code, str).msg;
        str.clear();
    }
    s = ":" + ip + " " + ft_itoa(code) + " " + Nick + " " + s + "\n";
    return s;
}
