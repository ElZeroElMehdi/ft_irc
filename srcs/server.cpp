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
    fcntl(this->server_fd, F_SETFL, O_NONBLOCK);
    if (bind(this->server_fd, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
        throw std::runtime_error("Bind failed, try again");
    std::cout << "Bind done" << std::endl;
}

void Server::listenSocket()
{
    listen(this->server_fd, -1);    
}

// void Server::acceptSocket()
// {
//     std::cout << "Accepting" << std::endl;
//     this->client_fd = accept(this->server_fd, (struct sockaddr *)&this->address, (socklen_t*)&this->address);
//     while (this->client_fd < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) // errno is forebeden to use in c++98 
//     {
//         std::cout << errno << std::endl;
//         this->client_fd = accept(this->server_fd, (struct sockaddr *)&this->address, (socklen_t*)&this->address);
//     }
//     std::cout << "Accepted" << std::endl;
//     std::cout << errno << std::endl;
//     if (this->client_fd < 0)
//         throw std::runtime_error("Accept failed");
//     send(this->client_fd, "Welcone to a7ssen irc dial triz tront sit say hello\n", 52, 0);
// }

void Server::acceptSocket()
{
    std::cout << "Accepting" << std::endl;
    struct pollfd fds[1];
    fds[0].fd = this->server_fd;
    fds[0].events = POLLIN;
    int ready = poll(fds, 1, 5000); // 5 second timeout
    if (ready == -1) {
        throw std::runtime_error("Poll error");
    }
    if (ready == 0) {
        throw std::runtime_error("Accept timed out");
    }
    if (fds[0].revents & POLLIN) {
        this->client_fd = accept(this->server_fd, (struct sockaddr *)&this->address, (socklen_t*)&this->addressLength);
        if (this->client_fd == -1) {
            throw std::runtime_error("Accept error");
        }
        // Set client socket to non-blocking
        int flags = fcntl(this->client_fd, F_GETFL, 0);
        fcntl(this->client_fd, F_SETFL, flags | O_NONBLOCK);
        send(this->client_fd, "Welcome to a7ssen irc dial triz tront sit say hello\n", 52, 0);
    } else {
        throw std::runtime_error("Unexpected poll event");
    }
    std::cout << "Accepted" << std::endl;
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

int Server::getSocketFd() const
{
    return this->server_fd;
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