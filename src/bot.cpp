#include "server.hpp"
#include <ctime>
#include <curl/curl.h>

static std::string current_time()
{
    time_t now = time(0);

    char *dt = ctime(&now);
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    std::string msg = "The UTC date and time is: " + std::string(dt);
    if (msg[msg.size() - 1] == '\n')
        msg.erase(msg.size() - 1);
    return (msg);
}

static std::vector<std::string> help()
{
    std::vector<std::string> msg;
    msg.push_back ("The following commands are available in server:\r");
    msg.push_back ("NICK <nickname> : sets your nickname\r");
    msg.push_back ("USER <username> <hostname> <servername> <realname> : sets your username\r");
    msg.push_back ("PASS <password> : sets your password\r");
    msg.push_back ("QUIT [message]> disconnects you from the server\r");
    msg.push_back ("PRIVMSG <channel> <message> : sends a message to a channel\r");
    msg.push_back ("NOTICE <channel> <message> : sends a notice to a channel\r");
    msg.push_back ("WHOIS <nickname> : gets information about a user\r");
    msg.push_back ("PING <server> : pings a server\r");
    msg.push_back ("PONG <server> : pongs a server\r");
    
    msg.push_back ("The following commands are available in channel:\r");
    msg.push_back ("JOIN <channel> : joins a channel\r");
    msg.push_back ("NAMES <channel> : gets a list of users in a channel\r");
    msg.push_back ("INVITE <nickname> <channel> : invites a user to a channel\r");
    msg.push_back ("LIST : gets a list of channels\r");
    msg.push_back ("TOPIC <channel> : gets the topic of a channel\r");
    msg.push_back ("MODE <channel> : gets the mode of a channel\r");
    msg.push_back ("KICK <channel> <nickname> : kicks a user from a channel\r");
    msg.push_back ("PART <channel> : leaves a channel\r");


    msg.push_back ("BOT <command> : sends a command to the bot\r");
    msg.push_back ("The following commands are available in bot>\r");
    msg.push_back ("TIME : gets the current time\r");
    msg.push_back ("HELP : gets a list of commands\r");
    msg.push_back ("ADHAN <city> : gets the adhan of a city\r");
    return (msg);
}

static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string* buffer = static_cast<std::string*>(userdata);
    buffer->append(ptr, size * nmemb);
    return size * nmemb;
}

static std::string adhanCity(std::string city)
{
    std::string value;
    curl_global_init(CURL_GLOBAL_ALL);
    // Create a CURL object.
    CURL* curl = curl_easy_init();
    std::string link = "https://voice.mediaplus.ma/api/v1/adan/Casablanca";
    if (!city.empty())
        link = "https://voice.mediaplus.ma/api/v1/adan/" + city;

    
    if (curl) {
        // Set the URL to fetch.
        curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
        // Set the callback function to handle the received data.
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        // Perform the HTTP request.
        CURLcode result = curl_easy_perform(curl);
        // Check for any errors.
        long http_response_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
        if (result == CURLE_OK && http_response_code == 404)
            value = "Resource not found" ;
        else if (result != CURLE_OK)
            value = "Error: " + std::string(curl_easy_strerror(result));
        else
            value = response;
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return value;
}

bool Server::irc_bot(int fd, s_command &c)
{
    std::string msg;
    std::vector<std::string> vars;
    if (c.target.size() == 0)
    {
        if (this->cl.find(fd)->second.getRegistred() == false)
            vars.push_back("");
        else
            vars.push_back(this->cl.find(fd)->second.getNick() + " ");
        vars.push_back("BOT");
        msg = this->showReply(461, fd, vars);
        send(fd, msg.c_str(), msg.size(), 0);
        vars.clear();
        return (false);
    }
    for (std::vector<std::string>::iterator it = c.target.begin(); it != c.target.end(); it++)
    {
        std::string tmp = str_toupper(*it);
        if (tmp == "TIME")
        {
            vars.push_back(this->getIp(this->fd_server));
            vars.push_back(current_time());
            msg = showReply(391, fd, vars);
            send(fd, msg.c_str(), msg.size(), 0);
            return (true);
        }
        else if (tmp == "HELP")
        {
            vars.clear();
            vars = help();
            for (std::vector<std::string>::iterator it = vars.begin(); it != vars.end(); it++)
            {
                std::vector<std::string> tmp;
                tmp.push_back(*it);
                msg = this->showReply(300, fd, tmp);
                send(fd, msg.c_str(), msg.size(), 0);
                tmp.clear();
            }
            return (true);
        }
        else if (tmp == "ADHAN")
        {
            if (c.target.size() != 1 || c.first_pram.size() == 0)
            {
                vars.push_back("ADHAN");
                msg = this->showReply(461, fd, vars);
                send(fd, msg.c_str(), msg.size(), 0);
                vars.clear();
                return (false);
            }
            std::string city = c.first_pram;
            std::string value = adhanCity(city);
            if (value.size() == 0)
            {
                vars.push_back(city);
                msg = this->showReply(300, fd, vars);
                send(fd, msg.c_str(), msg.size(), 0);
                return (false);
            }
            vars = splitString2(value, ",");
            std::vector<std::string> tmp;
            tmp.push_back("NOTICE : IF WRRONG CITY THIS API WHOULD DISPLAY A DEFAULT TIME (CASABLANCA-MOROCCO)\r");
            msg = this->showReply(300, fd, tmp);
            send(fd, msg.c_str(), msg.length(), 0);
            tmp.clear();
            for (std::vector<std::string>::iterator it = vars.begin(); it != vars.end(); it++)
            {
                tmp.push_back(*it);
                msg = this->showReply(300, fd, tmp);
                send(fd, msg.c_str(), msg.size(), 0);
                tmp.clear();
            }
            return (true);
        }
        else
        {
            vars.push_back(tmp);
            msg = this->showReply(421, fd, vars);
            send(fd, msg.c_str(), msg.size(), 0);
            return (false);
        }
    }
    return false;
}


bool Server::irc_send(int fd, s_command &c)
{
    (void)fd;
    if (c.target.size() == 0 || c.first_pram.size() == 0 || c.second_pram.size() == 0)
    {
        
        return (false);
    }
    return true;
}