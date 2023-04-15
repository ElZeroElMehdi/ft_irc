#ifndef INCLUDES_HPP
# define INCLUDES_HPP 

#include <iostream>

#include <sys/socket.h>
// #include <sys/types.h>  //not required in lunix
#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cstdlib>
#include <algorithm>

#include <vector>
#include <map>
#include <string.h>

#include "server.hpp"
#include "replies.hpp"

// #include "./channels/includes_channels.hpp"
#include "client.hpp"
#include "channels.hpp"
#include "utiles.hpp"
#include "commands.hpp"

// utills
std::string                 str_toupper(std::string &str);
std::vector<std::string>    splitString2(std::string str, const std::string delimiter);
std::string                 getToken(std::string line, int &i);
std::string                 replacer(std::string str, std::string s1, std::string s2);

#define DEBUG 1
#define DETAILS 0
#endif