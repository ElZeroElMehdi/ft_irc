#include "server.hpp"
#include "commands.hpp"

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



bool Server::irc_part(int fd, s_command &c)
{
    (void)fd;
    (void)c;
    return (true);
}

bool Server::irc_topic(int fd, s_command &c)
{
    (void)fd;
    (void)c;
    return (true);
}

bool Server::irc_invite(int fd, s_command &c)
{
    (void)fd;
    (void)c;
    return (true);
}

bool Server::irc_mode(int fd, s_command &c)
{
    (void)fd;
    (void)c;
    return (true);
}

bool Server::irc_kick(int fd, s_command &c)
{
    (void)fd;
    (void)c;
    return (true);
}

bool Server::irc_names(int fd, s_command &c)
{
    (void)fd;
    (void)c;
    return (true);
}

bool Server::irc_ban(int fd, s_command &c)
{
    (void)fd;
    (void)c;
    return (true);
}