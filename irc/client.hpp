#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>

class Channel;

class Client {
private:
    std::string username;
    int socket;
    std::vector<Channel*> channels;

public:
    Client(std::string username, int socket);
    std::string get_username() const;
    int get_socket() const;
    std::vector<Channel*> get_channels() const;
    void send_message(std::string message) const;
    void join_channel(Channel* channel);
    void leave_channel(Channel* channel);
};

#endif



