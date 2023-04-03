#ifndef SERVER_HPP
# define SERVER_HPP

#include "includes.hpp"

class Server{
    private:
        int fd_server;
        int port;
        std::string pw;
        struct sockaddr_in ConAddr;
        std::vector<pollfd> allFd;
    public:
        Server(int pt ,std::string password);
        //seters
        void setPort(int pr);
        //methods
        void createSocket();
        void bindSocket();
        void listenConix();
        void addFd(int fd);
        bool events();
        void creatServer();
        void chat();
        ~Server();
};

#endif