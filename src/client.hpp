#ifndef CLIENT_HPP
# define CLINET_HPP

#include "includes.hpp"

class Clinets
{
    private:
        int client_fd;
        std::string user;
        std::string nick;
        bool isRegistred;
        
    public:
        Clinets(int fd);
        Clinets(std::string user, std::string nick, int fd);
        void setNick(std::string nick);
        void setUser(std::string user);
        void setFd(int fd);
        void setRegistred(bool _isRegistred);
        std::string getNick();
        std::string getUser();
        int getFd();
        ~Clinets();
};


#endif