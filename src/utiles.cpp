#include "utiles.hpp"

std::string getTimeStamp() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

// char *getIpAddress(int sockfd) {
//     struct sockaddr_in client_addr;
//     socklen_t client_addr_len = sizeof(client_addr);
//     char client_ip[INET_ADDRSTRLEN];

//     if (getpeername(sockfd, (struct sockaddr*)&client_addr, &client_addr_len) == -1) {
//         perror("getpeername");
//     }

//     if (inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip)) == NULL) {
//         perror("inet_ntop");
//     }

//     printf("Client IP address: %s\n", client_ip);
//     return client_ip;
// }


std::vector<std::string> splitString(std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    std::string::size_type pos = 0, last_pos = 0;

    while ((pos = str.find(delimiter, last_pos)) != std::string::npos) {
        result.push_back(str.substr(last_pos, pos - last_pos));
        last_pos = pos + delimiter.size();
    }

    result.push_back(str.substr(last_pos));

    return result;
}

void removeSpaces(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}
