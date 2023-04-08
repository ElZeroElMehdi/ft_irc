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
            send(newClient, "welcome to my server\n", 21, 0);
            std::string s = "please registre first\n send:\n NICK <nick>\n\r USERNAME <username>\n\r for registertion\n";
            send(newClient, s.c_str(), s.length(), 0);
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
                if (cmd.substr(0, 4) == "QUIT" || cmd.substr(0, 4) == "quit")
                {
                    std::string s = "bye bye\n";
                    send(this->allFd[i].fd, s.c_str(), s.length(), 0);
                    close(this->allFd[i].fd);
                    this->allFd.erase(this->allFd.begin() + i);
                    this->cl.erase(this->allFd[i].fd);
                    break;
                }
                else if (cmd.substr(0, 4) == "join" || cmd.substr(0, 4) == "JOIN")
                {
                    this->cha.push_back(Channels(cmd.substr(5, cmd.length() - 5), this->cl.find(this->allFd[i].fd)->second.getFd()));
                    std::string s = "you join to " + cmd.substr(5, cmd.length() - 5) + "\n";
                    // this->cl.find(this->allFd[i].fd)->second.
                }
            //     std::cout << this->cl.find(this->allFd[i].fd)->second.getNick() << " : " << msg;
            //     for (size_t j = 0; j < this->allFd.size(); j++)
            //     {
            //         if (this->allFd.at(j).fd != this->fd_server && this->allFd.at(j).fd != this->allFd.at(i).fd && this->cl.find(this->allFd[j].fd)->second.getRegistred() == true)
            //         {
            //             std::string msge = this->cl.find(this->allFd[i].fd)->second.getNick() + " : " + msg;
            //             send(this->allFd[j].fd, msge.c_str(), msge.length(), 0);
            //         }
            //     }
            }
            else
            {
                std::string s = "please registre first\n send:\n NICK <nick>\n\r USERNAME <username>\n\r for registertion\n";
                send(this->allFd[i].fd, s.c_str(), s.length(), 0);
                recv(this->allFd[i].fd, msg, 1024, 0);
                std::string msgg = msg;
                //commands : NICK <nick> , USERNAME <username>
                if (msgg.substr(0, 4) == "NICK" || msgg.substr(0, 4) == "nick")
                    this->cl.find(this->allFd[i].fd)->second.setNick(msgg.substr(5, msgg.length() - 6));
                if (msgg.substr(0, 8) == "username" || msgg.substr(0, 8) == "USERNAME")
                    this->cl.find(this->allFd[i].fd)->second.setUser(msgg.substr(9, msgg.length() - 10));
                if(this->cl.find(this->allFd[i].fd)->second.checkIfRegistred())
                    send(this->allFd[i].fd, "you are registred\n", 18, 0);
            }
        }
        else if (this->allFd.at(i).revents & POLLHUP)
        {
            std::cout << "disconnected" << std::endl;
            close(this->allFd[i].fd);
            this->allFd.erase(this->allFd.begin() + i);
            this->cl.erase(this->allFd[i].fd);
        }
    }
}

Server::~Server()
{
    close(this->fd_server);
}

// commands

Server::Commands::Commands(std::string _command, std::vector<std::string> _param)
{
    this->command = _command;
    this->param = _param;
}

Server::Commands::Commands(std::string _command, Clinets &c)
{
    size_t pos = _command.find(" ");
    if (pos != std::string::npos && _command.substr(0, pos) == "nick")
        exit(0);
    else if (pos == std::string::npos && _command.substr(0, 4) == "nick")
    {
        std::cout << "plese " << std::endl;
        exit(0);
    }
}
void Server::Commands::nick(std::string _nick, Clinets &c)
{
    if (_nick.length() > 9)
        throw std::runtime_error("nick name is too long");
    c.setNick(_nick);
}

void Server::Commands::user(std::string _user, Clinets &c)
{
    c.setUser(_user);
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