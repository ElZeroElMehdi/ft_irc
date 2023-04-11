#ifndef UTILES_HPP
# define UTILES_HPP

# include "includes.hpp"
std::string getTimeStamp();
std::string generateMessage(std::string nickname, std::string ip_address, std::string server_name);
char *getIpAddress(int sockfd);
#endif