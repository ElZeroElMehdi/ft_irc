#include "server.hpp"
#include "commands.hpp"

void Server::save_user(std::vector<Channels>::iterator it, int fd, s_command c)
{
    it->addUser(this->cl.find(fd)->second);
    std::vector<std::string> tmp;
    std::string msg = ":" + this->getIp(fd) + ".ip " + "JOIN :" + it->getName() + "\n";
    sendToChannel(it->getName(), msg);
    if (it->getTopicSet())
    {
        tmp.push_back(it->getName());
        tmp.push_back(it->getTopic());
        msg = showReply(332, fd, tmp);
        send(fd, msg.c_str(), msg.length(), 0);
    }
    irc_names(fd, c);
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
                        {
                            if (it->getLimitSet())
                            {
                                if (it->getUsers().size() < (unsigned long)it->getLimit())
                                    save_user(it, fd, c);
                                else
                                {
                                    std::vector<std::string> tmp;
                                    tmp.push_back(it->getName());
                                    send(fd, showReply(471, fd, tmp).c_str(), showReply(471, fd, tmp).size(), 0);
                                    return true;
                                }
                            }
                            else
                                save_user(it, fd, c);
                        }
                        else
                        {
                            std::vector<std::string> tmp;
                            tmp.push_back(it->getName());
                            send(fd, showReply(475, fd, tmp).c_str(), showReply(475, fd, tmp).size(), 0);
                            return true;
                        }
                    }
                    else
                        save_user(it, fd, c);
                }
                else
                {
                    std::vector<std::string> tmp;
                    tmp.push_back(it->getName());
                    send(fd, showReply(473, fd, tmp).c_str(), showReply(473, fd, tmp).size(), 0);
                    return true;
                }
            }
            else
            {
                if (it->getKeySet())
                {
                    if (it->getKey() == key)
                        save_user(it, fd, c);
                    else
                    {
                        std::vector<std::string> tmp;
                        tmp.push_back(it->getName());
                        send(fd, showReply(475, fd, tmp).c_str(), showReply(475, fd, tmp).size(), 0);
                        return true;
                    }
                }
                else
                    save_user(it, fd, c);
            }
            return (true);
        }
    }
    return false;
}

void Server::sendToChannel(std::string channel, std::string message)
{
    for (std::vector<Channels>::iterator it = this->ch.begin(); it != this->ch.end(); ++it)
    {
        if (it->getName() == channel)
        {
            std::map<int, Clients> users = it->getUsers();
            for (std::map<int, Clients>::iterator it2 = users.begin(); it2 != users.end(); ++it2)
                send(it2->second.getFd(), message.c_str(), message.length(), 0);
            break;
        }
    }
}

int Server::irc_join(int fd, s_command &c)
{
    if (c.target.size() == 0)
    {
        //error
        std::vector<std::string> params;
        params.push_back(c.command);
        std::string error = showReply(461, fd, params);
        send(fd, error.c_str(), error.size(), 0);
        return false;
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
    // TODO: params size check
    if (c.target.size() == 0)
    {
        //error
        std::vector<std::string> params;
        params.push_back(c.command);
        std::string error = showReply(461, fd, params);
        send(fd, error.c_str(), error.size(), 0);
        return false;
        return 0;
    }
    for (std::vector<std::string>::iterator it = c.target.begin() ; it != c.target.end(); ++it)
    {
        for (std::vector<Channels>::iterator it2 = this->ch.begin(); it2 != this->ch.end(); ++it2)
        {
            if (it2->getName() == *it)
            {
                if (it2->getUsers().find(fd)->first == fd)
                {
                    it2->removeUser(this->cl.find(fd)->second);
                    if (it2->getUsers().size() == 0)
                        this->ch.erase(it2);
                    return (true);
                }
                //error 442
                std::vector<std::string> params;
                params.push_back(c.target[0]);
                std::string error = showReply(442, fd, params);
                send(fd, error.c_str(), error.size(), 0);
            }
        }
        //error 403
        std::vector<std::string> params;
        params.push_back(*it);
        send(fd, showReply(403, fd, params).c_str(), showReply(403, fd, params).size(), 0);
        return false;
    }
    return (true);
}

bool Server::irc_topic(int fd, s_command &c)
{
    // TODO: params size check
    if (c.target.size() == 0)
    {
        //error 461
        std::vector<std::string> params;
        params.push_back(c.command);
        std::string error = showReply(461, fd, params);
        send(fd, error.c_str(), error.size(), 0);
        return false;
    }
    if (c.target.size() > 1)
    {
        //error 403
        std::string channel = splitString2(c.original, " ")[1];
        std::vector<std::string> params;
        params.push_back(channel);
        send(fd, showReply(403, fd, params).c_str(), showReply(403, fd, params).size(), 0);
        return false;
    }
    for (std::vector<Channels>::iterator it = this->ch.begin(); it != this->ch.end(); ++it)
    {
        if (it->getName() == c.target[0])
        {
            if (it->getUsers().find(fd)->first == fd)
            {
                if (!c.second_pram.empty() || !c.first_pram.empty())
                {
                    if (!c.first_pram.empty())
                        it->setTopic(c.first_pram);
                    else
                        it->setTopic(c.second_pram);
                    std::vector<std::string> params;
                    params.push_back(c.target[0]);
                    params.push_back(it->getTopic());
                    std::string topic = showReply(332, fd, params);
                    sendToChannel(it->getName(), topic);
                    return (true);
                }
                else
                {
                    std::vector<std::string> params;

                    if (!it->getTopic().empty())
                    {
                        params.push_back(it->getName());
                        params.push_back(it->getTopic());
                        send(fd, showReply(332, fd, params).c_str(), showReply(332, fd, params).size(), 0);
                    }
                    else
                    {
                        params.push_back(it->getName());
                        send(fd, showReply(331, fd, params).c_str(), showReply(331, fd, params).size(), 0);
                    }
                    return (true);
                }
            }
            //error 442
            std::vector<std::string> params;
            params.push_back(c.target[0]);
            std::string error = showReply(442, fd, params);
            send(fd, error.c_str(), error.size(), 0);
            return false;
        }
    }
    return (true);
}

bool Server::irc_invite(int fd, s_command &c)
{
    std::vector<std::string> params;
    std::string channel = c.first_pram == "" ? c.second_pram : c.first_pram;
    if (c.target.size() == 0)
    {
        //error 461
        std::vector<std::string> params;
        params.push_back(c.command);
        std::string error = showReply(461, fd, params);
        send(fd, error.c_str(), error.size(), 0);
        return false;
    }
    if (c.target.size() > 1)
    {
        //error 403
        std::string channel = splitString2(c.original, " ")[1];
        std::vector<std::string> params;
        params.push_back(channel);
        send(fd, showReply(403, fd, params).c_str(), showReply(403, fd, params).size(), 0);
        return false;
    }
    // check if user is in channel
    for(std::vector<Channels>::iterator it = this->ch.begin(); it != this->ch.end(); ++it)
    {
        if(it->getName() == channel)
        {
            for (std::map<int, Clients>::iterator it2 = it->getUsers().begin(); it2 != it->getUsers().end(); ++it2)
            {
                if (it2->second.getNick() == c.target[0])
                {
                    //error 443
                    params.push_back(c.target[0]);
                    params.push_back(channel);
                    send(fd, showReply(443, fd, params).c_str(), showReply(443, fd, params).size(), 0);
                }
            }
        }
        else if (it++ == this->ch.end())
        {
            //error 403
            
            params.push_back(c.target[0]);
            send(fd, showReply(403, fd, params).c_str(), showReply(403, fd, params).size(), 0);
        }
    }
    for (std::map<int, Clients>::iterator it = this->cl.begin(); it != this->cl.end(); ++it)
    {
        if (it->second.getNick() == c.target[0])
        {
            params.push_back(c.target[0]);
            params.push_back(c.first_pram);
            std::string invite = showReply(341, fd, params);
            send(it->first, invite.c_str(), invite.size(), 0);
            return (true);
        }
    }
    //error 401
    params.push_back(c.target[0]);
    send(fd, showReply(401, fd, params).c_str(), showReply(401, fd, params).size(), 0);
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
    std::vector<std::string> params;
    params.push_back(c.target[0]);
    if (c.target.size() > 1)
    {
        //error 403
        std::string channel = splitString2(c.original, " ")[1];
        std::vector<std::string> params;
        params.push_back(channel);
        send(fd, showReply(403, fd, params).c_str(), showReply(403, fd, params).size(), 0);
        return false;
    }
    for (std::vector<Channels>::iterator it = this->ch.begin(); it != this->ch.end(); ++it)
    {
        if (it->getName() == c.target[0])
        {
            if (it->getUsers().find(fd)->first == fd)
            {
                std::map<int, Clients> users = it->getUsers();
                for (std::map<int, Clients>::iterator it2 = users.begin(); it2 != users.end(); ++it2)
                    params.push_back(it2->second.getNick());
            }
            else
            {
                std::vector<std::string> params;
                params.push_back(c.target[0]);
                std::string error = showReply(442, fd, params);
                send(fd, error.c_str(), error.size(), 0);
                return false;
            }
        }
    }
    if (params.size() == 1)
    {
        //error 403
        std::string channel = splitString2(c.original, " ")[1];
        std::vector<std::string> params;
        params.push_back(channel);
        send(fd, showReply(403, fd, params).c_str(), showReply(403, fd, params).size(), 0);
        return false;
    }
    else
    {
        for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); ++it)
        {
            // if user is op add @
            for (std::vector<Channels>::iterator it2 = this->ch.begin()+1; it2 != this->ch.end(); ++it2)
            {
                if (it2->getName() == c.target[0])
                {
                    if (it2->getOps().find(fd)->second.getNick() == *it)
                        *it = "@" + *it;
                    else
                        *it = "+" + *it;
                }
            }
        }
        std::string names = "";
        for (std::vector<std::string>::iterator it = params.begin()+1; it != params.end(); ++it)
            names += *it + " ";
        params.clear();
        params.push_back(c.target[0]);
        params.push_back(names);
        send(fd, showReply(353, fd, params).c_str(), showReply(353, fd, params).size(), 0);
        send(fd, showReply(366, fd, params).c_str(), showReply(366, fd, params).size(), 0);
    }
    return (true);
}

bool Server::irc_ban(int fd, s_command &c)
{
    (void)fd;
    (void)c;

    return (true);
}
