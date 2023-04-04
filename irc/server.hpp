#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include "client.hpp"
// #include "Channel.h"

class Server
{
private:
    std::string server_name;
    std::string server_desc;
    int port;
    int listen_socket;
    std::vector<Client *> connections;
    std::vector<Channel *> channels;

public:
    Server(std::string name, std::string desc, int port_num);
    ~Server();
    void start();
    void stop();
    void accept_connection();
    void broadcast(std::string msg);
    void join_channel(Client *client, std::string channel_name);
    void leave_channel(Client *client, std::string channel_name);
    Channel *get_channel(std::string channel_name);
};

#endif
