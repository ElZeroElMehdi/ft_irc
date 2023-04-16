#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "includes.hpp"
class Clients
{
    private:
        int client_fd;
        bool status;
        std::string user; //9 of char
        std::string SecendUser; //9 of char
        std::string nick;//9 of char
        bool isRegistred;
        std::string ip;
        unsigned int port;
        std::string pass;
        // std::vector<Channels> chanel;
    public:
        Clients(int fd);
        Clients(std::string user, std::string nick, int fd);
        void setNick(std::string nick);
        void setUser(std::string user);
        void setSecendUser(std::string user);
        void setPass(std::string pass);
        void setPort(unsigned int port);
        unsigned int getPort()const;
        std::string getSecendUser() const;
        void setFd(int fd);
        void setRegistred(bool _isRegistred);
        bool getRegistred() const;
        std::string getNick() const;
        bool getStatus() const;
        std::string getUser() const;
        std::string getPass() const;
        int getFd() const;
        std::string getIp() const;
        void setIp(std::string ip);
        // void addChannel(Channels channel);
        bool checkIfRegistred();
        ~Clients();
};


#endif