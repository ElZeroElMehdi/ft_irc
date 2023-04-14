#include "channels.hpp"
#include "includes_channels.hpp"

// // ============================ comands ============================

void check_invite_add_user(std::string cmd, Clients &user, std::map<std::string, Channels> &channels)
{
    if (channels.find(cmd)->second.getInviteSet() == true)
    {
        if (channels.find(cmd)->second.getInvited().find(user.getFd())->first != user.getFd())
            channels.find(cmd)->second.addUser(user);
        else
            std::cout << "User is not invited" << std::endl;
    }
    else
        channels.find(cmd)->second.addUser(user);
}

// void Channels::joinChannel(Clients &user)
// {    
//     if (this->getUsers().find(user.getFd())->first != user.getFd())
//     {
//         if (this->second.getKeySet() == true)
//         {
//             if (this->second.getKey() == cmd.first_pram)
//                 check_invite_add_user(*it, user, channels);
//             else
//                 std::cout << "Wrong key" << std::endl;
//         }
//         else
//             channels.find(*it)->second.addUser(user);
//     }
//     else
//         std::cout << "User is already in channel" << user.getFd() << user.getUser()  << std::endl;
// }

void partChannel(t_cmd cmd, Clients &user, std::map<std::string, Channels> &channels)
{
    for (std::vector<std::string>::iterator it = cmd.target.begin(); it != cmd.target.end(); ++it)
    {
        if (it->at(0) != '#')
        {
            std::cout << "Channel name must start with #" << channels.find(*it)->second.getName() << std::endl;
            return ;
        }
        if (channels.find(*it) != channels.end())
        {
            if (channels.find(*it)->second.getUsers().find(user.getFd())->first == user.getFd())
            {
                channels.find(*it)->second.removeUser(user);
                if (channels.find(*it)->second.getOps().find(user.getFd())->first == user.getFd())
                    channels.find(*it)->second.removeOp(user);
                if (channels.find(*it)->second.getModerators().find(user.getFd())->first == user.getFd())
                    channels.find(*it)->second.removeModerator(user);
            }
            else
                std::cout << "User is not in channel" << std::endl;
        }
        else
            std::cout << "Channel does not exist" << std::endl;
    }
}

void kickChannel(t_cmd cmd, Clients &user, Clients &kicked, std::map<std::string, Channels> &channels)
{
    if (cmd.target.size() == 0)
    {
        // err_code 461
        std::cout << "No channel name" << std::endl;
        return ;
    }
    // TODO: check if channel exists
    if (cmd.target[0].at(0) != '#')
    {
        std::cout << "Channel name must start with #" << channels.find(cmd.target[0])->second.getName() << std::endl;
        return ;
    }
    for (std::vector<std::string>::iterator it = cmd.target.begin(); it != cmd.target.end(); ++it)
    {
        if (channels.find(*it) != channels.end())
        {
            //TODO: check if user is op
            if (channels.find(*it)->second.getOps().find(user.getFd()) != channels.find(*it)->second.getOps().end())
            {
                // TODO: check if user is in channel
                if (channels.find(*it)->second.getUsers().find(kicked.getFd()) != channels.find(*it)->second.getUsers().end())
                {
                    // TODO: check if user is op or user or moderator and remove from all
                    if (channels.find(*it)->second.getOps().find(kicked.getFd()) != channels.find(*it)->second.getOps().end())
                        channels.find(*it)->second.removeOp(kicked);
                    if (channels.find(*it)->second.getUsers().find(kicked.getFd()) != channels.find(*it)->second.getUsers().end())
                        channels.find(*it)->second.removeUser(kicked);
                    if (channels.find(*it)->second.getModerators().find(kicked.getFd()) != channels.find(*it)->second.getModerators().end())
                        channels.find(*it)->second.removeModerator(kicked);
                    std::cout << "User : " << kicked.getUser() << " has been kicked from channel : " << *it << std::endl;
                }
            }
            else
                std::cout << "User : " << user.getUser() << " is not op in channel : " << *it << std::endl;
        }
        else
            std::cout << "Channel : " << *it << " does not exist" << std::endl;
    }
}

void topicChannel(t_cmd cmd, Clients &user, std::map<std::string, Channels> &channels)
{
    // TODO: check if channel exists
    if (cmd.target[0].at(0) != '#')
    {
        std::cout << "Channel name must start with #" << channels.find(cmd.target[0])->second.getName() << std::endl;
        return ;
    }
    if (channels.find(cmd.target[0]) != channels.end())
    {
        if (cmd.first_pram.empty() && cmd.second_pram.empty())
        {
            std::cout << "Channel Topic is : " << channels.find(cmd.target[0])->second.getTopic() << std::endl;
            return;
        }
        if (channels.find(cmd.target[0])->second.getTopicSet() == false)
        {
            channels.find(cmd.target[0])->second.setTopic(cmd.first_pram + cmd.second_pram);
            std::cout << "Channel Topic is : " << channels.find(cmd.target[0])->second.getTopic() << std::endl;
        }
        else
        {
            if (channels.find(cmd.target[0])->second.getOps().find(user.getFd()) != channels.find(cmd.target[0])->second.getOps().end())
            {
                channels.find(cmd.target[0])->second.setTopic(cmd.first_pram + " " + cmd.second_pram);
                channels.find(cmd.target[0])->second.setTopicSet(true);
            }
            else
                std::cout << "User : " << user.getUser() << " is not op in channel : " << cmd.target[0] << std::endl;
        }
    }
    else
        std::cout << "Channel : " << channels.find(cmd.target[0])->second.getName() << " does not exist" << std::endl;
}

void inviteChannel(t_cmd cmd, Clients &user, Clients &invited, std::map<std::string, Channels> &channels)
{
    // TODO: check if channel exists
    if (cmd.target[0].at(0) != '#')
    {
        std::cout << "Channel name must start with #" << channels.find(cmd.target[0])->second.getName() << std::endl;
        return ;
    }
    if (channels.find(cmd.target[0]) != channels.end())
    {
        if (channels.find(cmd.target[0])->second.getInviteSet() == false)
        {
            std::cout << "Channel : " << cmd.target[0] << " is not invite only" << std::endl;
            return;
        }
        if (channels.find(cmd.target[0])->second.getUsers().find(invited.getFd())->first == invited.getFd())
        {
            std::cout << "User : " << invited.getUser() << " is already in channel : " << cmd.target[0] << std::endl;
            return;
        }
        if (channels.find(cmd.target[0])->second.getInvited().find(invited.getFd())->first == invited.getFd())
        {
            std::cout << "User : " << invited.getUser() << " has already been invited to channel : " << cmd.target[0] << std::endl;
            return;
        }
        if (channels.find(cmd.target[0])->second.getOps().find(user.getFd())->first != user.getFd())
        {
            std ::cout << "Invited : " << invited.getUser() << " is not operator in channel : " << cmd.target[0] << std::endl;
            return;
        }
        channels.find(cmd.target[0])->second.addInvited(invited);
        std::cout << "User : " << invited.getUser() << " has been invited to channel : " << cmd.target[0] << std::endl;
    }
    else
        std::cout << "Channel : " << channels.find(cmd.target[0])->second.getName() << " does not exist" << std::endl;
}

// // void modeChannel(std::string channel, std::string user, std::string mode, std::string modeuser, std::vector<Channels> &cha)+
// // {
// //     // TODO: check if channel exists
// //     int channelindex = channelExists(channel, cha);
// //     if (channelindex != -1)
// //     {
// //         // TODO: check if user is op
// //         if (user)
// //     }
// //     else
// //         std::cout << "Channel : " << channel << " does not exist" << std::endl;
// // }
// // comand in channel join #channel, part #channel, topic #channel, mode #channel, kick #channel, invite #channel, privmsg #channel  