#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "includes.hpp"


struct s_command;

class Commands
{
    private:
        std::vector<s_command> list;
    public:
        Commands(std::string _cmd);
        const std::vector<s_command> &getList()const;
};

struct s_command
{
    std::string                 original;
    
    std::string                 command;
    std::vector<std::string>    target;
    std::string                 first_pram;
    std::string                 second_pram;
};

struct IRCCommand
{
    std::string name;
    bool        need_registred;
};

std::vector<IRCCommand> commands_list();
bool                    commands_check(std::string cmd);
IRCCommand              command_info(std::string cmd);
bool                    validator(s_command &c);
void                    parsParms(s_command &c, std::string params);
bool                    irc_nick(s_command &c);
bool                    command_routes(s_command &c);
#endif