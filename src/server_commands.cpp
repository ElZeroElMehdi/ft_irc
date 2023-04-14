#include "server.hpp"
#include "commands.hpp"

int Server::irc_user(int fd, s_command &c)
{
    if (this->cl.find(fd)->second.getRegistred() == true)
    {
        std::string error = showReply(462, fd, c.target);
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
        std::string error = this->showReply(461, fd, tmp);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    this->cl.find(fd)->second.setSecendUser(c.second_pram.substr(pos, c.second_pram.length() - pos));
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
    if (c.target.size() && !c.target[0].empty())
    {
        if (!isValidNickname(fd, c.target[0]))
            return (0);
        this->cl.find(fd)->second.setNick(c.target[0]);
        std::cout << "Client nick to " << c.target[0] << std::endl;
        this->cl.find(fd)->second.checkIfRegistred();
    }
    else
    {
        std::string error = showReply(431, fd, c.target);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    return (1);
}

/*
whois MEHDIEL0
:punch.wa.us.dal.net 311 eelmoham MEHDIEL0 ~MEHDI 5c8c-aff4-7127-3c3-1c20.230.197.ip * : MEHDI200
:punch.wa.us.dal.net 319 eelmoham MEHDIEL0 :@#channelMehdi
:punch.wa.us.dal.net 312 eelmoham MEHDIEL0 punch.wa.us.dal.net : Shovel ready since 1998!
:punch.wa.us.dal.net 317 eelmoham MEHDIEL0 77 1681434548 :seconds idle, signon time
:punch.wa.us.dal.net 318 eelmoham MEHDIEL0 :End of /WHOIS list.

*/

bool Server::irc_whois(int fd, s_command &c)
{
    
    std::vector<std::string> tmp;
    std::string error;
    if (c.target.size() == 0)
    {
        error = this->showReply(431, this->fd_server, tmp);
        send(fd, error.c_str(), error.length(), 0);
        return 0;
    }
    for (std::vector<std::string>::iterator it = c.target.begin(); it != c.target.end(); ++it)
    {
        std::string nick = *it;
        if (this->findClinet(nick) == -1)
        {
            // replis 401 & 318
            tmp.push_back(nick);
            error = this->showReply(401, fd, tmp);
            send(fd, error.c_str(), error.length(), 0);
            error.clear();
            error = this->showReply(318, fd, tmp);
            send(fd, error.c_str(), error.length(), 0);
            error.clear();
            tmp.clear();
            return 0;
        }
        else
        {
            // 319
            tmp.push_back(nick);
            // tmp.push_back(nick);
            tmp.push_back("~" + this->cl.find(this->findClinet(nick))->second.getUser());
            tmp.push_back(this->getIp(this->cl.find(this->findClinet(nick))->second.getFd())+ ".ip");
            tmp.push_back(this->cl.find(this->findClinet(nick))->second.getSecendUser());
            error = this->showReply(311, fd, tmp);
            send(fd, error.c_str(), error.length(), 0);
            error.clear();
            tmp.clear();

            //...
            
            tmp.push_back(nick);
            tmp.push_back(this->getIp(this->fd_server));
            tmp.push_back("ircserver created by eelmoham oakouda and oalaoui- students at 1337 school");
            error = this->showReply(312, fd, tmp);
            send(fd, error.c_str(), error.length(), 0);
            error.clear();
        }
    }
    return 1;
}

bool Server::find_channel(std::string &name, int fd, s_command c)
{
    std::string key = c.first_pram != "" ? c.first_pram : c.second_pram;

    for (std::vector<Channels>::iterator it = this->ch.begin(); it != this->ch.end(); ++it)
    {
        if (it->getName() == name)
        {
            if (it->getUsers().find(fd)->first == fd)
                return true;
            if (it->getInviteSet())
            {
                if (it->getInvited().find(fd)->first == fd)
                {
                    if (it->getKeySet())
                    {
                        if (it->getKey() == key)
                            it->addUser(this->cl.find(fd)->second);
                        else
                        {
                            std::cout << "wrong key" << std::endl;
                            return true;
                        }
                    }
                    else
                        it->addUser(this->cl.find(fd)->second);
                }
            }
            else
            {
                if (it->getKeySet())
                {
                    if (it->getKey() == key)
                        it->addUser(this->cl.find(fd)->second);
                    else
                    {
                        std::cout << "wrong key" << std::endl;
                        return true;
                    }
                }
                else
                    it->addUser(this->cl.find(fd)->second);
            }
            return (true);
        }
    }
    return false;
}



int Server::irc_join(int fd, s_command &c)
{
    if (c.target.size() == 0)
    {
        //error
        return 0;
    }
    std::vector<std::string> &targs = c.target; 
    for (std::vector<std::string>::iterator it = targs.begin(); it != targs.end(); ++it)
    {
        if (!find_channel(*it, fd, c))
        {
            std::string pass = "";
            pass = c.first_pram;
            if (pass.empty())
                pass = c.second_pram;
            Channels newChannel(*it ,pass);
            newChannel.addUser(this->cl.find(fd)->second);
            newChannel.addOp(this->cl.find(fd)->second);
            ch.push_back(newChannel);
        }
    }
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
        std::cout << "Client pass to " << c.target[0] << std::endl;
        if (this->isPass(c.target[0]) == true)
        {
            this->cl.find(fd)->second.setPass(c.target[0]);
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

bool Server::irc_privmsg(int fd, s_command &c)
{
    std::cout << "privmsg done!" << std::endl;
    if (this->findClinet(this->cl.find(fd)->second.getNick()) != fd)
    {
        std::string error = showReply(401, fd, c.target);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    return 1;
}

// bool Server::irc_quit(int fd, s_command &c)
// {
//     // std::cout << "quit done!" << std::endl;
// }

