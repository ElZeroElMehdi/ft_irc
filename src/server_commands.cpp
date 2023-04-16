#include "server.hpp"
#include "commands.hpp"

void Server::welcome(int fd, s_command &c)
{
    Clients &user = this->cl.find(fd)->second;
    if (!user.getNick().empty() && !user.getUser().empty() && !user.getPass().empty() && !user.getRegistred() && !user.getStatus())
    {
        std::string msg = showReply(1, fd, c.target);
        send(fd, msg.c_str(), msg.length(), 0);
        msg = showReply(2, fd, c.target);
        send(fd, msg.c_str(), msg.length(), 0);
        msg = showReply(3, fd, c.target);
        send(fd, msg.c_str(), msg.length(), 0);
        msg = showReply(4, fd, c.target);
        send(fd, msg.c_str(), msg.length(), 0);
    }
}

int Server::irc_user(int fd, s_command &c)
{
    if (c.target.size() == 0)
    {   std::vector<std::string> tmp;
        tmp.push_back("USER");
        std::string error = showReply(461, fd, tmp);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    if (this->cl.find(fd)->second.getRegistred() == true)
    {
        std::string error = showReply(462, fd, c.target);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    if (c.target[0] == "*")
    {
        std::vector<std::string> tmp;
        tmp.push_back("*");
        tmp.push_back("USER");
        std::string error = this->showReply(468, fd, tmp);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    this->cl.find(fd)->second.setUser(c.target[0]);
    int pos = c.second_pram.find(" :");
    if(pos == -1)//std::string::npos
        pos = c.second_pram.find(" ");
    if (pos == -1)
    {
        std::vector<std::string> tmp;
        tmp.push_back("USER");
        std::string error = this->showReply(461, fd, tmp);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    this->cl.find(fd)->second.setSecendUser(c.second_pram.substr(pos, c.second_pram.length() - pos));
    welcome(fd, c);
    this->cl.find(fd)->second.checkIfRegistred();
    
    return (1);
}

static bool include(char needle, std::string stack)
{
    std::cout << "needle: " << stack << std::endl;
    for (std::string::const_iterator it = stack.begin(); it != stack.end(); ++it)
    {
        if (*it == needle)
            return true;
    }
    return false;
}

bool Server::isValidNickname(int fd, const std::string &nickname)
{
    // Check length
    std::vector<std::string> vars;
    vars.push_back(nickname);

    if (nickname.empty() || nickname.length() > 30)
    {
        std::string error = showReply(432, fd, vars);
        send(fd, error.c_str(), error.length(), 0);
        return false;
    }

    // Check first character
    if (!std::isalpha(nickname[0]) && nickname[0] != '_' && !include(nickname[0], "[\\]`^{|}"))
    {
        std::string error = showReply(432, fd, vars);
        send(fd, error.c_str(), error.length(), 0);
        return false;
    }

    // Check remaining characters
    for (std::string::const_iterator it = nickname.begin() + 1; it != nickname.end(); ++it)
    {
        if (!std::isalnum(*it) && *it != '_' && *it != '-' && !include(*it, "[\\]`^{|}#@"))
        {
            std::string error = showReply(432, fd, vars);
            send(fd, error.c_str(), error.length(), 0);
            return false;
        }
    }

    return true;
}

int Server::irc_nick(int fd, s_command &c)
{
    std::string error;
    if (c.target.size() && !c.target[0].empty())
    {
        if (this->cl.find(fd)->second.getNick() == c.target[0])
            return 1;
        if (!isValidNickname(fd, c.target[0]))
            return (0);
        if(this->isUserEx(this->findClinet(c.target[0])))
        {
            std::vector<std::string> tmp;
            tmp.push_back(c.target[0]);
            error = this->showReply(433, fd, tmp);
            send(fd, error.c_str(), error.length(), 0);
            return (0);
        }
        else
        {
            if (this->cl.find(fd)->second.getRegistred() == true)
            {
                std::string msg = ":" + this->cl.find(fd)->second.getNick() + "!~" + this->cl.find(fd)->second.getUser() + "@" + this->cl.find(fd)->second.getIp() + ".ip NICK :" + c.target[0] + "\n";
                send(fd, msg.c_str(), msg.length(), 0);
            }
            this->cl.find(fd)->second.setNick(c.target[0]);
            welcome(fd, c);
            this->cl.find(fd)->second.checkIfRegistred();
            return (1);
        }
    }
    else
    {
        error = showReply(431, fd, c.target);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    return (1);
}

bool Server::irc_whois(int fd, s_command &c)
{
    
    std::vector<std::string> tmp;
    std::string error;
    if (c.target.size() == 0)
    {
        error = this->showReply(431, fd, tmp);
        send(fd, error.c_str(), error.length(), 0);
        return 0;
    }
    for (std::vector<std::string>::iterator it = c.target.begin(); it != c.target.end(); ++it)
    {
        std::string nick = *it;
        if (this->findClinet(nick) == -1)
        {
            tmp.push_back(nick);
            error = this->showReply(401, fd, tmp);
            send(fd, error.c_str(), error.length(), 0);
            error.clear();
            tmp.clear();
            tmp.push_back(nick);
            error = this->showReply(318, fd, tmp);
            send(fd, error.c_str(), error.length(), 0);
            error.clear();
            tmp.clear();
            return 0;
        }
        else
        {
            tmp.push_back(nick);
            tmp.push_back("~" + this->cl.find(this->findClinet(nick))->second.getUser());
            tmp.push_back(this->getIp(this->cl.find(this->findClinet(nick))->second.getFd())+ ".ip");
            tmp.push_back(this->cl.find(this->findClinet(nick))->second.getSecendUser());
            error = this->showReply(311, fd, tmp);
            send(fd, error.c_str(), error.length(), 0);
            error.clear();
            tmp.clear();
            std::string Chs = searchChannelsByUser(nick);
            std::cout << "Chs: " << Chs << std::endl;
            if (!Chs.empty())
            {
                tmp.push_back(nick);
                tmp.push_back(Chs);
                error = this->showReply(319, fd, tmp);
                send(fd, error.c_str(), error.length(), 0);
                tmp.clear();
            }
            tmp.push_back(nick);
            tmp.push_back(this->getIp(this->fd_server));
            tmp.push_back("ircServer created by oakoudad eelmoham and oalaoui- students at 1337 school");
            error = this->showReply(312, fd, tmp);
            send(fd, error.c_str(), error.length(), 0);
            error.clear();
        }
    }
    tmp.clear();
    tmp.push_back(*(c.target.begin()));
    error = this->showReply(318, fd, tmp);
    send(fd, error.c_str(), error.length(), 0);
    return 1;
}

bool Server::irc_pass(int fd, s_command &c)
{
    if (this->cl.find(fd)->second.getRegistred() == true)
    {
        std::string error = showReply(462, fd, c.target);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    else if (c.target.size() && !c.target[0].empty())
    {
        if (this->isPass(c.target[0]) == true)
        {
            this->cl.find(fd)->second.setPass(c.target[0]);
            welcome(fd, c);
            this->cl.find(fd)->second.checkIfRegistred();
            return (1);
        }
        else
        {
            std::string error = showReply(464, fd, c.target);
            send(fd, error.c_str(), error.length(), 0);
            return (0);
        }
    }
    else
    {
        std::string error = showReply(461, fd, c.target);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    return (1);
}

bool Server::irc_privmsg_notice(int fd, s_command &c)
{
    std::vector<std::string> tmp;
    std::string error;
    if (c.target.size())
    {
        if (c.first_pram.empty() && c.second_pram.empty())
        {
            error = this->showReply(412, fd, tmp);
            send(fd, error.c_str(), error.length(), 0);
            return (0);
        }
        for(std::vector<std::string>::iterator it = c.target.begin();it != c.target.end();++it)
        {
            //check if  *it channel or user
            std::string msg = ":"+this->cl.find(fd)->second.getNick()+"!~"+this->cl.find(fd)->second.getUser()+"@"+this->getIp(fd)+" " + str_toupper(c.command) + " "+(*it)+" :";
            if (!c.first_pram.empty())
                msg += c.first_pram + "\n";
            else
            {
                if (c.second_pram.substr(0, 1) == ":")
                    msg += c.second_pram.substr(1) + "\n";
                else
                    msg += c.second_pram + "\n";
            }            
            if ((*it)[0] == '#')
            {
                sendToChannel(*it, msg);
                continue;
            }
            
            int to = this->findClinet(*it);
            if (to == -1)
            {
                msg = this->showReply(401, fd, c.target) + "\n";
                send(fd, msg.c_str(), msg.length(), 0);
                continue;
            }
            send(to, msg.c_str(), msg.length(), 0);
        }
        return 1;
    }
    error = showReply(461, fd, c.target);
    send(fd, error.c_str(), error.length(), 0);
    return (0);
}



bool Server::irc_list(int fd, s_command &c)
{
    (void)c;
    //321 RPL_LISTSTART
    std::vector<std::string> vars;

    std::string replay = showReply(321, fd, vars);
    send(fd, replay.c_str(), replay.length(), 0);
    vars.clear();
    //322 RPL_LIST
    for(std::vector<Channels>::iterator it = this->ch.begin(); it != this->ch.end(); ++it){
        vars.push_back(it->getName());
        vars.push_back(ft_itoa(it->getUsers().size()));
        vars.push_back(it->getTopic());
        replay = showReply(322, fd, vars);
        send(fd, replay.c_str(), replay.length(), 0);
        vars.clear();
    }
    //323 RPL_LISTEND
    replay = showReply(323, fd, vars);
    send(fd, replay.c_str(), replay.length(), 0);
    return true;
}

bool Server::irc_quit(int fd, s_command &c)
{
    std::vector<std::string> tmp;
    std::string error;
    if (c.target.size() && !c.target[0].empty())
    {
        if (this->cl.find(fd)->second.getRegistred() == true)
            error = "ERROR : Closing Link: " + this->getIp(fd) + " (Quit: " + c.target[0] + ")";
        else
            error = "ERROR : Closing Link: 0.0.0.0 (Quit: " + c.target[0] + ")";
    }
    else if(this->cl.find(fd)->second.getRegistred() == true)
        error = "ERROR : Closing Link: " + this->getIp(fd) + " (Quit: " + this->cl.find(fd)->second.getNick() +")";
    else
        error = "ERROR : Closing Link: 0.0.0.0 (Quit: " ")";
    error +="\n";
    send(fd, error.c_str(), error.length(), 0);
    close(fd);
    this->cl.erase(fd);
    return (1);
}

bool Server::irc_pong(int fd, s_command &c)
{
    std::string rep;
    if (c.target.size() == 0)
    {
        std::vector<std::string> vars;
        vars.push_back(str_toupper(c.command));
        rep = showReply(461, fd, vars);
        send(fd, rep.c_str(), rep.length(), 0);
        return (0);
    }
    return (1);
}

std::string str_join(std::vector<std::string> &v, std::string delim)
{
    std::string s;
    for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it)
    {
        s += *it;
        if (it + 1 != v.end())
            s += delim;
    }
    return (s);
}

bool Server::irc_ping(int fd, s_command &c)
{
    std::string rep;
    if (c.target.size() == 0)
    {
        std::vector<std::string> vars;
        vars.push_back(str_toupper(c.command));
        rep = showReply(461, fd, vars);
        send(fd, rep.c_str(), rep.length(), 0);
        return (0);
    }
    if (c.original.find(":") != std::string::npos && c.target.size() == 0)
        rep = ":" + this->getIp(fd) + " PONG " + this->getIp(fd) + " :" + c.original.substr(c.original.find(":") + 1) + "\n";
    else
        rep = ":" + this->getIp(fd) + " PONG " + this->getIp(fd) + " :" + str_join(c.target, ",") + "\n";
    send(fd, rep.c_str(), rep.length(), 0);
    return (1);
}
