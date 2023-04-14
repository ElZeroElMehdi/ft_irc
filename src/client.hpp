#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "includes.hpp"
class Clinets
{
    private:
        int client_fd;
        std::string user; //9 of char
        std::string SecendUser; //9 of char
        std::string nick;//9 of char
        bool isRegistred;
        std::string ip;
        std::string pass;
        // std::vector<Channels> chanel;
    public:
        Clinets(int fd);//maybe not need
        Clinets(std::string user, std::string nick, int fd);
        void setNick(std::string nick);
        void setUser(std::string user);
        void setSecendUser(std::string user);
        void setPass(std::string pass);
        std::string getSecendUser() const;
        void setFd(int fd);
        void setRegistred(bool _isRegistred);
        bool getRegistred() const;
        std::string getNick() const;
        std::string getUser() const;
        int getFd() const;
        std::string getIp() const;
        void setIp(std::string ip);
        // void addChannel(Channels channel);
        bool checkIfRegistred();
        ~Clinets();
};


#endif