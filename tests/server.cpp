#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd, newsockfd, portno, clilen, n;
    char buffer[MAX_INPUT];
    struct sockaddr_in serv_addr, cli_addr;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        std::cerr << "Error opening socket\n";
        return 1;
    }

    // Set server address and port number
    portno = 1234;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error binding socket\n";
        return 1;
    }

    // Listen for incoming connections
    listen(sockfd, 5);

    // Accept incoming connections
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, (socklen_t*) &clilen);
    if (newsockfd < 0) {
        std::cerr << "Error accepting connection\n";
        return 1;
    }

    // Receive and send data
    std::memset(buffer, 0, sizeof(buffer));
    n = read(newsockfd, buffer, sizeof(buffer));
    if (n < 0) {
        std::cerr << "Error reading from socket\n";
        return 1;
    }
    std::cout << "Client: " << buffer << "\n";
    std::memset(buffer, 0, sizeof(buffer));
    std::cout << "Please enter a message: ";
    std::cin >> buffer;
    n = write(newsockfd, buffer, strlen(buffer));
    if (n < 0) {
        std::cerr << "Error writing to socket\n";
        return 1;
    }

    // Close sockets
    close(newsockfd);
    close(sockfd);

    return 0;
}
