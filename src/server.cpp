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
        throw std::runtime_error("creating socket faild!");
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
        throw std::runtime_error("no connection comming!");
    }
}

void Server::addFd(int fd, struct sockaddr_in Cl)
{
    struct pollfd newFd;
    newFd.fd = fd;
    newFd.events = POLLIN;
    this->allFd.push_back(newFd);
    if (fd == this->fd_server)
        return;
    Clients newClient(fd);
    newClient.setPort(ntohs(Cl.sin_port));
    newClient.setIp(std::string(inet_ntoa(Cl.sin_addr)));
    newClient.setIp(std::string(inet_ntoa(Cl.sin_addr)));
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
        struct sockaddr_in ClinetAddr;
        int len = sizeof(ClinetAddr);
        int newClient = accept(this->fd_server, (struct sockaddr *)&ClinetAddr, (socklen_t *)&len);
        if (newClient == -1)
        {
            close(this->fd_server);
            return false;
        }

        this->addFd(newClient, ClinetAddr);
    }
    return true;
}

void Server::creatServer()
{
    this->createSocket();
    this->bindSocket();
    this->listenConix();
    this->addFd(this->fd_server, this->ConAddr);
    while (1)
    {
        if (this->events())
            this->chat();
    }
}

int Server::findClinet(std::string nickName)
{
    for (std::map<int, Clients>::iterator it = this->cl.begin(); it != this->cl.end(); it++)
    {
        if (it->second.getNick() == nickName)
            return it->first;
    }
    return -1;
}

bool Server::isUserEx(int fd)
{
    if (this->cl.find(fd) != this->cl.end())
        return true;
    return false;
}

Server::~Server()
{
    for (std::map<int, Clients>::iterator it = this->cl.begin(); it != this->cl.end(); it++)
        close(it->first);
    close(this->fd_server);
}

void Server::chat()//other name like runServer
{
    for (size_t i = 0; i < this->allFd.size(); i++)
    {
        Clients &user = this->cl.find(this->allFd[i].fd)->second;
        if (this->allFd.at(i).revents & POLLIN && (this->allFd.at(i).fd != this->fd_server))
        {
            char msg[1024];
            memset(msg, 0, 1024);
            recv(this->allFd[i].fd, msg, 1024, 0);
            std::string cmd = msg;

            Commands newCmds(cmd);
            std::vector<s_command> tmp = newCmds.getList();
            for (std::vector<s_command>::iterator it = tmp.begin(); it != tmp.end(); ++it)
            {
                if (command_routes(this->allFd[i].fd, *it) == -1 && user.getRegistred())
                {
                    std::vector<std::string> str;
                    str.push_back(it->command);
                    std::string msg = this->showReply(421, this->allFd[i].fd, str);
                    send(this->allFd[i].fd, msg.c_str(), msg.length(), 0);
                }
            }
        }
        if (this->allFd.at(i).revents & POLLHUP)
        {
            close(user.getFd());
            this->allFd.erase(this->allFd.begin() + i);
            this->cl.erase(user.getFd());
            // this->irc_quit(user.getFd(), );
        }
    }
}

int Server::command_routes(int fd, s_command &c)
{
    IRCCommand info = command_info(c.command);
    bool registred = this->cl.find(fd)->second.getRegistred();

    if (info.name == "NICK")
        return this->irc_nick(fd, c);
    if (info.name == "USER")
        return this->irc_user(fd, c);
    if (info.name == "PASS")
        return this->irc_pass(fd, c);
    if (info.name == "QUIT")
        return this->irc_quit(fd, c);
    if(info.name == "BOT")
        return this->irc_bot(fd, c);
    if (registred == true)
    {
        if (info.name == "WHOIS")
            return this->irc_whois(fd, c);
        if (info.name == "PRIVMSG" || info.name == "NOTICE")
            return this->irc_privmsg_notice(fd, c);
        if (info.name == "JOIN")
            return this->irc_join(fd, c);
        if (info.name == "PART")
            return this->irc_part(fd, c);
        if (info.name == "TOPIC")
            return this->irc_topic(fd, c);
        if (info.name == "INVITE")
            return this->irc_invite(fd, c);
        if (info.name == "MODE")
            return this->irc_mode(fd, c);
        if (info.name == "KICK")
            return this->irc_kick(fd, c);
        if (info.name == "NAMES")
            return this->irc_names(fd, c);
        if (info.name == "LIST")
            return this->irc_list(fd, c);
        if (info.name == "PONG")
            return this->irc_pong(fd, c);
        if (info.name == "PING")
            return this->irc_ping(fd, c);
    }
    return (-1);
}

std::string Server::showReply(int code, int fd, std::vector<std::string> &vars)
{
    std::string ip = this->getIp(fd);
    std::string Nick = this->cl.find(fd)->second.getNick();
    std::string User = this->cl.find(fd)->second.getUser();

    std::string s;
    std::vector<std::string> str;
    if (code == 1)
    {
        s = Nick + "," + User + "," + ip;
        str = splitString(s, ",");
        s = get_replay(code, str).msg;
        str.clear();
    }
    else if (code == 2)
    {
        s = ip + ", v2.0";
        str = splitString(s, ",");
        s = get_replay(code, str).msg;
        str.clear();
    }
    else if (code == 3)
    {
        str.push_back("2023-04-01");
        s = get_replay(code, str).msg;
        str.clear();
    }
    else if (code == 4)
    {
        str.push_back(Nick);
        str.push_back(ip);
        str.push_back("v2.0");
        s = get_replay(code, str).msg;
        str.clear();
    }
    else
        s = get_replay(code, vars).msg;
    s = ":" + ip + " " + ft_rep_code(code) + " " + Nick + " " + s + "\n";
    return s;
}


std::string Server::getIp(int fd)
{
    if(fd == this->fd_server)
        return "localhost";
    else
        return this->cl.find(fd)->second.getIp();
}

bool Server::isPass(std::string pass)
{
    if (pass == this->pw)
        return true;
    return false;
}


std::string Server::searchChannelsByUser(std::string &user)
{
    std::string channels = "";
    for (std::vector<Channels>::iterator it = this->ch.begin(); it != ch.end(); ++it)
    {
        std::cout << "CH : "<< it->getName() << std::endl;
        if (it->isInChannel(user))
        {
            if (it->isOpt(user))
                channels += "@" + it->getName() + " ";
            else
                channels += it->getName() + " ";
        }
    }
    return channels;
}