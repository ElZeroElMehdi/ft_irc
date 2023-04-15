#include "server.hpp"
#include <ctime>

static std::string current_time()
{
    time_t now = time(0);

    char *dt = ctime(&now);
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    std::string msg = "The UTC date and time is: " + std::string(dt);
    return (msg);
}

static std::string help()
{
    // print all command available in my server

    std::string msg = "The following commands are available in server:\n";
    msg += "NICK <nickname> : sets your nickname\n";
    msg += "USER <username> <hostname> <servername> <realname> : sets your username\n";
    msg += "PASS <password> : sets your password\n";
    msg += "QUIT [message]: disconnects you from the server\n";
    msg += "JOIN <channel> : joins a channel\n";
    msg += "PART <channel> : leaves a channel\n";
    msg += "PRIVMSG <channel> <message> : sends a message to a channel\n";
    msg += "NOTICE <channel> <message> : sends a notice to a channel\n";
    msg += "WHOIS <nickname> : gets information about a user\n";
    msg += "NAMES <channel> : gets a list of users in a channel\n";
    msg += "LIST : gets a list of channels\n";
    msg += "TOPIC <channel> : gets the topic of a channel\n";
    msg += "BOT <command> : sends a command to the bot\n";
    msg += "The following commands are available in bot:\n";
    msg += "TIME : gets the current time\n";
    msg += "HELP : gets a list of commands\n";
    return (msg);
}

bool Server::irc_bot(int fd, s_command &c)
{
    std::string msg;
    std::vector<std::string> vars;
    if (c.target.size() == 0)
    {
        vars.push_back("BOT");
        msg = this->showReply(461, fd, vars);
        send(fd, msg.c_str(), msg.size(), 0);
        return (false);
    }
    for (std::vector<std::string>::iterator it = c.target.begin(); it != c.target.end(); it++)
    {
        if (*it == "TIME")
        {
            msg = current_time() + "\n";
            msg += ":" + this->getIp(this->fd_server) + " 371 :" + msg;
            send(fd, msg.c_str(), msg.size(), 0);
            return (true);
        }
        else if (*it == "HELP")
        {
            msg = help();
            send(fd, msg.c_str(), msg.size(), 0);
            return (true);
        }
        else
        {
            vars.push_back(*it);
            msg = this->showReply(421, fd, vars);
            send(fd, msg.c_str(), msg.size(), 0);
            return (false);
        }
    }
    return false;
}