#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <vector>
# include <map>
#include <iostream>
class Clients
{
    private:
        int client_fd;
        std::string user; //9 of char
        std::string nick;//9 of char
        bool isRegistred;
    public:
        Clients(int fd);//maybe not need
        Clients(std::string user, std::string nick, int fd);
        void setNick(std::string nick);
        void setUser(std::string user);
        void setFd(int fd);
        void setRegistred(bool _isRegistred);
        bool getRegistred() const;
        std::string getNick() const;
        std::string getUser() const;
        int getFd() const;
        // void addChannel(Channels channel);
        bool checkIfRegistred();
        ~Clients();
};


#endif