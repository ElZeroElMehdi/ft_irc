#include "commands.hpp"

Commands::Commands(std::string _cmd)
{
    int i;
    _cmd = replacer(_cmd, "\r\n", "\n");
    std::vector<std::string> splitedCommands = splitString2(_cmd, "\n");
    for (std::vector<std::string>::iterator it = splitedCommands.begin(); it != splitedCommands.end(); it++)
    {
        i = 0;
        s_command c;

        c.original = *it;
        c.command = getToken(*it, i);
        if (i != -1)
        {
            while ((*it)[i] == ' ')
                i++;
            if ((*it)[i] == ':')
                i++;
            c.target = splitString2(getToken(*it, i), ",");
        }
        if (i != -1)
            parsParms(c, (*it).substr(i));
        this->list.push_back(c);
    }

    if (DEBUG)
        for(std::vector<s_command>::iterator cmmd = this->list.begin(); cmmd != this->list.end(); cmmd++)
        {
            std::cout << "org : " << cmmd->original << std::endl;
            if (DETAILS){
                std::cout << "cmd : " << cmmd->command << std::endl;
                for(std::vector<std::string>::iterator iv = cmmd->target.begin(); iv != cmmd->target.end(); iv++)
                    std::cout << "Tar : " << *iv << std::endl;
                std::cout << std::endl;
                std::cout << "Pr1 : '" << cmmd->first_pram << "'" << std::endl;
                std::cout << "Pr2 : '" << cmmd->second_pram << "'" << std::endl;
                std::cout << "__________________________" << std::endl;
            }
        }
}

const std::vector<s_command> &Commands::getList()const
{
    return (this->list);
}