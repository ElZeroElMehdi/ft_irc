#include "includes.hpp"
#include "commands.hpp"

void    parsParms(s_command &c, std::string params)
{
    int i = 0;

    while (params[0] != '\0' && params[0] == ' ')
        params = params.substr(1);
    
    if (params[0] == ':')
        c.second_pram = params.substr(1);
    else
    {
        c.first_pram = getToken(params, i);
        if (i != -1){
            if (params[i] == ':')
                c.second_pram = params.substr(i + 1);
            else
                c.second_pram = params.substr(i);
        }
    }
}



std::vector<IRCCommand> commands_list()
{
    static std::vector<IRCCommand> rep;

    rep.push_back((IRCCommand){"NICK",      1});
    rep.push_back((IRCCommand){"USER",      1});
    rep.push_back((IRCCommand){"PASS",      1});
    rep.push_back((IRCCommand){"QUIT",      1});
    rep.push_back((IRCCommand){"BOT",       1});

    // need to be registred
    rep.push_back((IRCCommand){"JOIN",      0});
    rep.push_back((IRCCommand){"PART",      0});
    rep.push_back((IRCCommand){"PRIVMSG",   0});
    rep.push_back((IRCCommand){"NOTICE",    0});
    rep.push_back((IRCCommand){"WHOIS",     0});
    rep.push_back((IRCCommand){"NAMES",     0});
    rep.push_back((IRCCommand){"LIST",      0});
    rep.push_back((IRCCommand){"MODE",      0});
    rep.push_back((IRCCommand){"TOPIC",     0});
    rep.push_back((IRCCommand){"PONG",      0});
    rep.push_back((IRCCommand){"PING",      0});
    rep.push_back((IRCCommand){"INVITE",    0});
    rep.push_back((IRCCommand){"KICK",      0});
    return rep;
}

bool commands_check(std::string cmd)
{
    std::vector<IRCCommand> cmds = commands_list();

    for(std::vector<IRCCommand>::iterator obj = cmds.begin(); obj != cmds.end(); obj++)
    {
        if ((*obj).name == str_toupper(cmd))
            return (true);
    }
    // send err : 421
    std::cout << "localhost 421 oakoudad " << cmd << " :Unknown command" << std::endl;
    return (false);
}

IRCCommand  command_info(std::string cmd)
{
    std::vector<IRCCommand> cmds = commands_list();

    for(std::vector<IRCCommand>::iterator obj = cmds.begin(); obj != cmds.end(); obj++)
    {
        if ((*obj).name == str_toupper(cmd))
            return (*obj);
    }
    return ((IRCCommand){"", 0});
}

// bool validator(s_command &c)
// {
//     if (!commands_check(c.command))
//         return 0;
//     return command_routes(c);
// }
