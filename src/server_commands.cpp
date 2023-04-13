#include "server.hpp"
#include "commands.hpp"

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

bool include(char needle, std::string stack)
{
    std::cout << "needle: " << stack << std::endl;
    for (std::string::const_iterator it = stack.begin(); it != stack.end(); ++it) {
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

    if (nickname.empty() || nickname.length() > 30) {
        std::string error = showReply(432, fd, vars);
        send(fd, error.c_str(), error.length(), 0);
        return false;
    }

    // Check first character
    if (!std::isalpha(nickname[0]) && nickname[0] != '_' && !include(nickname[0], "[\\]`^{|}")) {
        std::string error = showReply(432, fd, vars);
        send(fd, error.c_str(), error.length(), 0);
        return false;
    }

    // Check remaining characters
    for (std::string::const_iterator it = nickname.begin() + 1; it != nickname.end(); ++it) {
        if (!std::isalnum(*it) && *it != '_' && *it != '-' && !include(*it, "[\\]`^{|}")) {
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
    }
    else
    {
        std::string error = showReply(431, fd, c.target);
        send(fd, error.c_str(), error.length(), 0);
        return (0);
    }
    return (1);
}