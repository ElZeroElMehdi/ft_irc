#include "channels.hpp"

Channels::Channels()
{
    this->name = "";
    this->topic = "";
    this->mode = "";
    this->key = "";

    this->topicSet = false;
    this->modeSet = false;
    this->keySet = false;
    this->scretSet = true;
    this->inviteSet = false;
    this->outsideSet = true;
}

Channels::Channels(std::string name, std::string key)
{
    this->name = name;
    this->key = key;

    this->topicSet = false;
    this->modeSet = false;
    if (key != "")
        this->keySet = true;
    else
        this->keySet = false;
    this->scretSet = false;
    this->inviteSet = false;
    this->outsideSet = false;
}

Channels::~Channels() {}

// ============================ getters ============================

std::string Channels::getName() const
{
    return this->name;
}

std::string Channels::getTopic() const
{
    return this->topic;
}

std::string Channels::getMode() const
{
    return this->mode;
}

std::string Channels::getKey() const
{
    return this->key;
}

// ============================ setters ============================

void Channels::setName(std::string name)
{
    this->name = name;
}

void Channels::setTopic(std::string topic)
{
    this->topic = topic;
}

void Channels::setMode(std::string mode)
{
    this->mode = mode;
}

void Channels::setKey(std::string key)
{
    this->key = key;
}

void Channels::setTopicSet(bool topicSet)
{
    this->topicSet = topicSet;
}

void Channels::setModeSet(bool modeSet)
{
    this->modeSet = modeSet;
}

void Channels::setKeySet(bool keySet)
{
    this->keySet = keySet;
}

void Channels::setScretSet(bool scretSet)
{
    this->scretSet = scretSet;
}

void Channels::setInviteSet(bool inviteSet)
{
    this->inviteSet = inviteSet;
}

void Channels::setOutsideSet(bool outsideSet)
{
    this->outsideSet = outsideSet;
}

// // ============================ getters bool ============================

bool Channels::getTopicSet() const
{
    return this->topicSet;
}

bool Channels::getModeSet() const
{
    return this->modeSet;
}

bool Channels::getKeySet() const
{
    return this->keySet;
}

bool Channels::getScretSet() const
{
    return this->scretSet;
}

bool Channels::getInviteSet() const
{
    return this->inviteSet;
}

bool Channels::getOutsideSet() const
{
    return this->outsideSet;
}

// // ============================ getters vector ============================

std::map<int, Clients> Channels::getUsers() const
{
    return this->users;
}

std::map<int, Clients> Channels::getOps() const
{
    return this->ops;
}

std::map<int, Clients> Channels::getModerators() const
{
    return this->moderators;
}

std::map<int, Clients> Channels::getInvited() const
{
    return this->invited;
}

// ============================ setters vector ============================

void Channels::addUser(Clients &user)
{
    if (this->users.find(user.getFd()) == this->users.end())
        this->users.insert(std::pair<int, Clients>(user.getFd(), user));
}
void Channels::addOp(Clients &op)
{
    if (this->ops.find(op.getFd()) == this->ops.end())
        this->ops.insert(std::pair<int, Clients>(op.getFd(), op));
    std::cout << "Op : " << this->ops.find(op.getFd())->second.getUser() << " added to channel : " << this->name << std::endl;
}

void Channels::addModerator(Clients &moderator)
{
    if (this->moderators.find(moderator.getFd()) == this->moderators.end())
        this->moderators.insert(std::pair<int, Clients>(moderator.getFd(), moderator));
}

void Channels::addInvited(Clients &invited)
{
    if (this->invited.find(invited.getFd()) == this->invited.end())
        this->invited.insert(std::pair<int, Clients>(invited.getFd(), invited));
}

// // ============================ remove ============================

void Channels::removeUser(Clients& user)
{
    if (this->users.find(user.getFd()) != this->users.end())
        this->users.erase(user.getFd());
}

void Channels::removeOp(Clients& op)
{
    if (this->ops.find(op.getFd()) != this->ops.end())
        this->ops.erase(op.getFd());
}

void Channels::removeModerator(Clients& moderator)
{
    if (this->moderators.find(moderator.getFd()) != this->moderators.end())
        this->moderators.erase(moderator.getFd());
}

void Channels::removeInvited(Clients& invited)
{
    if (this->invited.find(invited.getFd()) != this->invited.end())
        this->invited.erase(invited.getFd());
}
