#ifndef SERVER_HPP
#define SERVER_HPP

#include "includes.hpp"

struct s_command;
class Clients;
class Channels;

class Server
{
    private:
        int fd_server;
        int port;
        std::string pw;
        struct sockaddr_in ConAddr;
        std::vector<pollfd> allFd;
        std::map<int, Clients> cl;
        std::vector<Channels> ch;

    public:
        Server(const char *pt, const char *password);
        // seters
        void setPort(int pr);
        int getPort() const;
        bool isPass(std::string pass);
        // methods
        void createSocket();
        void bindSocket();
        void listenConix();
        void addFd(int fd, struct sockaddr_in Cl);
        bool events();
        void creatServer();
        void chat();
        bool isUserEx(int fd);
        int findClinet(std::string nickName);
        std::string showReply(int code, int fd, std::vector<std::string> &vars);
        std::string getIp(int fd);


        //validations
        int command_routes(int fd, s_command &c);
        //commands

        int     irc_nick(int fd, s_command &c);
        bool    isValidNickname(int fd, const std::string &nickname);
        bool    find_channel(std::string &name, int fd, s_command c);

        int irc_user(int fd, s_command &c);
        int irc_join(int fd, s_command &c);
        bool irc_whois(int fd, s_command &c);
        bool irc_pass(int fd, s_command &c);
        // bool irc_ping(int fd, s_command &c);
        // bool irc_pong(int fd, s_command &c);
        bool irc_privmsg_notice(int fd, s_command &c);
        // bool irc_part(int fd, s_command &c);
        bool irc_quit(int fd, s_command &c);
        bool irc_part(int fd, s_command &c);
        bool irc_topic(int fd, s_command &c);
        bool irc_invite(int fd, s_command &c);
        bool irc_mode(int fd, s_command &c);
        bool irc_kick(int fd, s_command &c);
        bool irc_names(int fd, s_command &c);
        bool irc_ban(int fd, s_command &c);

        bool irc_list(int fd, s_command &c);
        void sendToChannel(std::string channel, std::string message);
        void save_user(std::vector<Channels>::iterator it, int fd, s_command c);
        //for new nranch
        
        //welcome
        void welcome(int fd, s_command &c);

        ~Server(); 
};
#endif