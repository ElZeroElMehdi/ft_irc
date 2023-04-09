#ifndef SERVER_HPP
#define SERVER_HPP

#include "includes.hpp"
#include "client.hpp"
#include "channels.hpp"
class Server
{
    private:
        int fd_server;
        int port;
        std::string pw;
        struct sockaddr_in ConAddr;
        std::vector<pollfd> allFd;
        std::map<int, Clinets> cl;
        std::vector<Channels> cha;

    public:
        Server(int pt, std::string password);
        // seters
        void setPort(int pr);
        // methods
        void createSocket();
        void bindSocket();
        void listenConix();
        void addFd(int fd);
        bool events();
        void creatServer();
        void chat();
        int findClinet(std::string nickName);
        ~Server(); 
};

#endif