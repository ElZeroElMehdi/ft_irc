#include "server.hpp"
#include <ctime>

static std::string current_time()
{
    time_t now = time(0);

    char *dt = ctime(&now);
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    std::string msg = "The UTC date and time is: " + std::string(dt);
    if (msg[msg.size() - 1] == '\n')
        msg.erase(msg.size() - 1);
    return (msg);
}

std::vector<std::string> help()
{
    std::vector<std::string> msg;
    msg.push_back ("The following commands are available in server:\r");
    msg.push_back ("NICK <nickname> : sets your nickname\r");
    msg.push_back ("USER <username> <hostname> <servername> <realname> : sets your username\r");
    msg.push_back ("PASS <password> : sets your password\r");
    msg.push_back ("QUIT [message]> disconnects you from the server\r");
    msg.push_back ("JOIN <channel> : joins a channel\r");
    msg.push_back ("PART <channel> : leaves a channel\r");
    msg.push_back ("PRIVMSG <channel> <message> : sends a message to a channel\r");
    msg.push_back ("NOTICE <channel> <message> : sends a notice to a channel\r");
    msg.push_back ("WHOIS <nickname> : gets information about a user\r");
    msg.push_back ("NAMES <channel> : gets a list of users in a channel\r");
    msg.push_back ("LIST : gets a list of channels\r");
    msg.push_back ("TOPIC <channel> : gets the topic of a channel\r");
    msg.push_back ("BOT <command> : sends a command to the bot\r");
    msg.push_back ("The following commands are available in bot>\r");
    msg.push_back ("TIME : gets the current time\r");
    msg.push_back ("HELP : gets a list of commands\r");
    return (msg);
}

bool Server::irc_bot(int fd, s_command &c)
{
    std::string msg;
    std::vector<std::string> vars;
    if (c.target.size() == 0)
    {
        if (this->cl.find(fd)->second.getRegistred() == false)
            vars.push_back("");
        else
            vars.push_back(this->cl.find(fd)->second.getNick() + " ");
        vars.push_back("BOT");
        msg = this->showReply(461, fd, vars);
        send(fd, msg.c_str(), msg.size(), 0);
        vars.clear();
        return (false);
    }
    for (std::vector<std::string>::iterator it = c.target.begin(); it != c.target.end(); it++)
    {
        std::string tmp = str_toupper(*it);
        if (tmp == "TIME")
        {
            vars.push_back(this->getIp(this->fd_server));
            vars.push_back(current_time());
            msg = showReply(391, fd, vars);
            send(fd, msg.c_str(), msg.size(), 0);
            return (true);
        }
        else if (tmp == "HELP")
        {
            vars.clear();
            vars = help();
            for (std::vector<std::string>::iterator it = vars.begin(); it != vars.end(); it++)
            {
                std::vector<std::string> tmp;
                tmp.push_back(*it);
                msg = this->showReply(300, fd, tmp);
                send(fd, msg.c_str(), msg.size(), 0);
                tmp.clear();
            }
            return (true);
        }
        else
        {
            vars.push_back(tmp);
            msg = this->showReply(421, fd, vars);
            send(fd, msg.c_str(), msg.size(), 0);
            return (false);
        }
    }
    return false;
}