#ifndef UTILES_HPP
# define UTILES_HPP

# include "includes.hpp"
std::string getTimeStamp();
std::string generateMessage(std::string nickname, std::string ip_address, std::string server_name);
char *getIpAddress(int sockfd);
std::vector<std::string> splitString(std::string& str, const std::string& delimiter);
void removeSpaces(std::string& str);
std::string ft_rep_code(int num);
#endif