#ifndef REPLIES_HPP
#define REPLIES_HPP

#include "includes.hpp"

struct IRCReply {
    int         code;
    std::string slog;
    std::string msg;
    std::string desc;
};

IRCReply get_replay( int code, std::vector<std::string> vars );

#endif