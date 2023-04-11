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

std::string generateMessage(std::string nickname, std::string ip_address, std::string server_name){
    std::string message;

    std::cout << nickname << " | " << ip_address << " | " << server_name << std::endl;
    message += "*** Looking up your hostname...\n*** Found your hostname\nLogged in\n:"+server_name+": Welcome to the " + server_name + " IRC Network " + nickname + "@" + ip_address + "\nYour host is " + server_name + ", running version bahamut-2.2.2\nThis server was created " + getTimeStamp() + "\n";
    message += server_name + " bahamut-2.2.2 aAbcCdefFghHiIjkKmnoOPrRsSwxXy AbceiIjklLmMnoOpPrRsStv beIjklov\n";
    message += "NETWORK=DALnet SAFELIST MAXBANS=200 MAXCHANNELS=50 CHANNELLEN=32 KICKLEN=307 NICKLEN=30 TOPICLEN=307 MODES=6 CHANTYPES=# CHANLIMIT=#:50 PREFIX=(ohv)@%+ STATUSMSG=@%+ are available on this server\nCASEMAPPING=ascii WATCH=128 SILENCE=10 ELIST=cmntu EXCEPTS INVEX CHANMODES=beI,k,jl,ciPAmMnOprRsSt MAXLIST=b:200,e:100,I:100 TARGMAX=DCCALLOW:,JOIN:,KICK:4,KILL:20,NOTICE:20,PART:,PRIVMSG:20,WHOIS:,WHOWAS: are available on this server\n";
    message += "There are 89 users and 7229 invisible on 39 servers\n80 IRC Operators online\n3863 channels formed\nI have 1 client and 1 server\n";
    message += "Current local users: 1 Max: 2105\nCurrent global users: 7318 Max: 12060\n*** Notice -- motd was last changed at " + getTimeStamp() + "\n";
    message += "*** Notice -- Please read the motd if you haven't read it\n- " + server_name + " Message of the Day -\n-\n- Welcome to " + server_name + "! This is " + server_name + ", and YOU are\n- the reason " + server_name + " is here!\n-\n-                     SERVER RULES:                  \n-                                                      \n-       IMPERSONATION, CLONING, SPAMMING, FLOODING,    \n-             OR ANY OTHER KIND OF ABUSE CAN            \n-        AND WILL CAUSE YOU TO BE BANNED FROM THIS      \n-                SERVER AT ANY GIVEN TIME              \n-\n- We reserve the right to deny access to this server without\n- warning or explanation.  Usage is governed by the " + server_name + " AUP at <http://www." + server_name + ".net/aup/>.\n-\n-   * For help:                      /join #help              *\n-   * For IRCop/CSop assistance:     /join #OperHelp          *\n-   * For multilingual information:  <http://docs." + server_name + ".net/>   *\n-  ";
    
    return message;
}