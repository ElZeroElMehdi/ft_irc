#include "command.hpp"
 //utils
#include <vector>
#include <string>

std::vector<std::string> splitString(std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    std::string::size_type pos = 0, last_pos = 0;

    while ((pos = str.find(delimiter, last_pos)) != std::string::npos) {
        result.push_back(str.substr(last_pos, pos - last_pos));
        last_pos = pos + delimiter.size();
    }

    result.push_back(str.substr(last_pos));

    return result;
}

#include <algorithm>
#include <cctype>

void removeSpaces(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}


Commands::Commands(std::string _cmd)
{
    std::vector<std::string> splitedCommands = splitString(_cmd, "\r\n");
    for(std::vector<std::string>::iterator it = splitedCommands.begin(); it != splitedCommands.end(); it++)
    {
        cmd c;
        int pos = it->find(":");
        if (pos != std::string::npos)
        {
            c.msg = it->substr(pos+1, it->length());
            it->erase(pos, it->length());
        }
        c.command = it->substr(0, it->find(" "));
        removeSpaces(c.command);
        it->erase(0, c.command.length());
        c.target = splitedCommands(*it, " ");
    }
}

// void Commands::nick(std::string _nick, Clinets &c)
// {
//     if (_nick.length() > 9)
//         throw std::runtime_error("nick name is too long");
//     c.setNick(_nick);
// }

// void Commands::user(std::string _user, Clinets &c)
// {
//     c.setUser(_user);
// }