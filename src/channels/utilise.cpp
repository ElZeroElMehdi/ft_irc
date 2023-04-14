#include "channels.hpp"

// bool channelExists(std::string channel, std::map<std::string, Channels> channels)
// {
//     if (channels.find(channel) != channels.end())
//         return true;
//     return false;
// }

// void check_inveted(std::pair<std::string, Channels> &channel, std::pair<int, Clients> user)
// {
//     if (channel.second.getInviteSet())
//     {
//         // TODO: check if user is invited
//         if (channel.second.getInvited().find(user.first) != channel.second.getInvited().end())
//             channel.second.addUser(user);
//         else
//             std::cout << "User : " << user.second.getUser() << " is not invited to channel : " << channel.first << std::endl;
//     }
//     else
//     {
//         // TODO: add user to channel
//         channel.second.addUser(user);
//         // std::cout << "User : " << channel.second.getUsers().find(user.first)->second.getUser() << " added to channel : " << channel.first << std::endl;
//     }
// }

// bool check_ops(Channels channel, std::pair<int, Clients> user)
// {
//     // TODO: check if user is op
//     if (channel.getOps().find(user.first) != channel.getOps().end())
//         return true;
//     return false;
// }
    