#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "client.hpp"
#include <string>
#include <vector>


class Channels
{
    private:
        int limit;
        unsigned long timestamp;
        std::string name;
        std::string topic;
        std::string mode;
        std::string key;

        bool topicSet;
        bool visibleSet;
        bool limitSet;
        bool modeSet;
        bool keySet;
        bool scretSet;
        bool inviteSet;
        bool outsideSet;

        std::map<int, Clients> users;
        std::map<int, Clients> ops;
        std::map<int, Clients> moderators;
        std::map<int, Clients> invited;

    public:
        Channels();
        Channels(std::string name, std::string key);
        ~Channels();

        int getLimit() const;
        unsigned long getTimestamp() const;
        std::string getName() const;
        std::string getTopic() const;
        std::string getMode() const;
        std::string getKey() const;

        std::map<int, Clients> *getUsers();
        std::map<int, Clients> *getOps();
        std::map<int, Clients> getModerators() const;
        std::map<int, Clients> getInvited() const;

        bool getTopicSet() const;
        bool getVisibleSet() const;
        bool getLimitSet() const;
        bool getModeSet() const;
        bool getKeySet() const;
        bool getScretSet() const;
        bool getInviteSet() const;
        bool getOutsideSet() const;

        bool isInChannel(std::string &user);
        bool isOpt(std::string &user);

        void setName(std::string name);
        void setTimestamp(unsigned long timestamp);
        void setTopic(std::string topic);
        void setMode(std::string mode);
        void setKey(std::string key);
        void setTopicSet(bool topicSet);
        void setModeSet(bool modeSet);
        void setKeySet(bool keySet);
        void setScretSet(bool scretSet);
        void setInviteSet(bool inviteSet);
        void setOutsideSet(bool outsideSet);
        void setLimitSet(bool limitSet);
        void setVisibleSet(bool visibleSet);
        void addmode(std::string mode);

        void addUser(Clients user);
        void addOp(Clients op);
        void addModerator(Clients &moderator);
        void addInvited(Clients &invited);

        void removeUser(Clients& user);
        void removeOp(Clients& op);
        void removeModerator(Clients& moderator);
        void removeInvited(Clients& invited);
        void rm(int fd);

};

// comand in channel join #channel, part #channel, topic #channel, mode #channel, kick #channel, invite #channel, privmsg #channel
// void joinChannel(t_cmd cmd, Clients &user, std::map<std::string, Channels> &channels);
// void partChannel(t_cmd cmd, Clients &user, std::map<std::string, Channels> &channels);
// void topicChannel(t_cmd cmd, Clients &user, std::map<std::string, Channels> &channels);
// // void modeChannel(std::string channel, std::string user, std::string mode);
// void kickChannel(t_cmd cmd, Clients &user, Clients &kicked, std::map<std::string, Channels> &channels);
// void inviteChannel(t_cmd cmd, Clients &user, Clients &invited, std::map<std::string, Channels> &channels);
// void privmsgChannel(std::string channel, std::string user, std::string message);

// utilise
// bool channelExists(t_cmd cmd, std::pair<int, Clients> user, std::vector<Channels> &cha);
// void check_inveted(std::pair<std::string, Channels>& channel, std::pair<int, Clients> user);
// bool check_ops(Channels channel, std::pair<int, Clients> user);
#endif