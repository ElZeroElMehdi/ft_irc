#include "server.hpp"

Server::Server(int _port, std::string _password):password(_password), port(_port)
{
    if (this->port < 1024 || this->port > 49151) //probably not the best way to do this but it works for now
        throw std::invalid_argument("Port must be between 1024 and 49151");
}

void Server::createSocket()
{
    this->server_fd = socket(AF_INET, SOCK_STREAM, 0);// ad_INET = ipv4, SOCK_STREAM = TCP, 0 = default protocol
    if (this->server_fd == -1)
        throw std::runtime_error("Socket creation failed");
    std::cout << "Socket created" << std::endl;
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->port);
    setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->addressLength, sizeof(this->addressLength));
    if (bind(this->server_fd, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
        throw std::runtime_error("Bind failed, try again");
    std::cout << "Bind done" << std::endl;
}

void Server::listenSocket()
{
    listen(this->server_fd, 3);    
}

void Server::acceptSocket()
{
    this->client_fd = accept(this->server_fd, (struct sockaddr *)&this->address, (socklen_t*)&this->address);
    if (this->client_fd < 0)
        throw std::runtime_error("Accept failed");
    send(this->client_fd, "Welcone to a7ssen irc dial triz tront sit say hello\n", 52, 0);
}

void Server::readSocket()
{
    char buffer[1024] = {0};
    int valread = read(this->client_fd, buffer, 1024);
    if (valread < 0)
        throw std::runtime_error("Read failed");
    this->buffer = buffer;
}

void Server::writeSocket()
{
    std::cout << "Buffer: " << this->buffer << std::endl;
    if (this->buffer == "hello\n")
        send(this->client_fd, "world", 5, 0);
    else
        send(this->client_fd, "error", 5, 0);
    close(this->client_fd);
}

Server::~Server()
{
    close(this->server_fd);
    std::cout << "Closing socket" << std::endl;
}

void Server::run()
{
    this->createSocket();
    this->listenSocket();
    while (1)
    {
        this->acceptSocket();
        this->readSocket();
        this->writeSocket();
    }
    this->~Server();
}