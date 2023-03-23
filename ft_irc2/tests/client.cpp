#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[256];

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

    // Connect to server
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error connecting to server\n";
        return 1;
    }

    // Send and receive data
    std::cout << "Please enter a message: ";
    std::memset(buffer, 0, sizeof(buffer));
    std::cin >> buffer;
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        std::cerr << "Error writing to socket\n";
        return 1;
    }
    std::memset(buffer, 0, sizeof(buffer));
    n = read(sockfd, buffer, sizeof(buffer));
    if (n < 0) {
        std::cerr << "Error reading from socket\n";
        return 1;
    }
    std::cout << "Server: " << buffer << "\n";

    // Close socket
    close(sockfd);

    return 0;
}
