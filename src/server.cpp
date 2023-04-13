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
        std::string s = ":localhost NOTICE AUTH :*** Looking up your hostname...\n:localhost NOTICE AUTH :*** Found your hostname\n";
        send(newClient, s.c_str(), s.length(), 0); // send a msg to the client as reply to the connection request
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
            recv(this->allFd[i].fd, msg, 1024, 0);
            std::string cmd = msg;

            Commands newCmds(cmd);
            std::vector<s_command> tmp = newCmds.getList();
            if (this->cl.find(this->allFd[i].fd)->second.getRegistred() == false)
            {
                // just nick and user
            }
            else
            {
                for (std::vector<s_command>::iterator it = tmp.begin(); it != tmp.end(); ++it)
                {
                    if (command_routes(this->allFd[i].fd, *it) == -1)
                    {
                        std::vector<std::string> str;
                        str.push_back(it->command);
                        std::string msg = this->showReply(421, this->allFd[i].fd, str);
                        send(this->allFd[i].fd, msg.c_str(), msg.length(), 0);
                    }
                }
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

int Server::command_routes(int fd, s_command &c)
{
    IRCCommand info = command_info(c.command);
    // bool registred = this->cl.find(fd)->second.checkIfRegistred();

    if (info.name == "NICK")
        return this->irc_nick(fd, c);
    if (info.name == "USER")
        return this->irc_user(fd, c);
    return (-1);
}

std::string Server::showReply(int code, int fd, std::vector<std::string> &vars)
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
    else if (code == 5)
    {
        str.push_back(ip);
        s = get_replay(code, str).msg;
        str.clear();
    }
    else
    {
        s = get_replay(code, vars).msg;
        s = ":" + ip + " " + Nick + " " + ft_itoa(code) + " " + s + "\n";
        return s;
    }
    s = ":" + ip + " " + ft_itoa(code) + " " + Nick + " " + s + "\n";
    return s;
}

int Server::irc_user(int fd, s_command &c)
{
    this->cl.find(fd)->second.setUser(c.target[0]);
    int pos = c.second_pram.find(":");
    if (pos == -1)
    {
        std::vector<std::string> tmp;
        std::string error = this->showReply(461, fd, tmp);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    this->cl.find(fd)->second.setSecendUser(c.second_pram.substr(pos, c.second_pram.length() - pos));
    return (1);
}

int Server::irc_nick(int fd, s_command &c)
{
    if (!c.target[0].empty())
        this->cl.find(fd)->second.setNick(c.target[0]);
    else
    {
        std::string error = showReply(431, fd, c.target);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    return (1);
}