#ifndef SERVER_HPP
# define SERVER_HPP

#include "includes.hpp"
#include "client.hpp"
class Server{
    private:
        int fd_server;
        int port;
        std::string pw;
        struct sockaddr_in ConAddr;
        std::vector<pollfd> allFd;
        std::map<int, Clinets> cl;

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
        class Commands
        {
            private:
                std::string command;
                std::vector<std::string> param;
            public:
                Commands(std::string _command, std::vector<std::string> _param);
                Commands(std::string _command, Clinets &c);
                void nick(std::string _nick, Clinets &c);
                void user(std::string _user, Clinets &c);
        };
        ~Server();
};

#endif