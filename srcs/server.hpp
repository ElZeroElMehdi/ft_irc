#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <poll.h>

class Server 
{
    private:
        int server_fd, client_fd, socketFd, port;
        socklen_t addressLength;
        std::string buffer, password;
        struct sockaddr_in address;
    public:
        Server(int _port, std::string _password);
        void createSocket();
        void listenSocket();
        void acceptSocket();
        void readSocket();
        void writeSocket();
        void run();
        int getSocketFd() const;
        ~Server();
};
#endif