#include "replies.hpp"

IRCReply    replay_list( int code )
{
    std::vector<IRCReply> rep;
    //
    rep.push_back((IRCReply){001, "RPL_WELCOME", "Welcome to the Internet Relay Network $1!~$2@$3", "n/a"});
    rep.push_back((IRCReply){002, "RPL_YOURHOST", "Your host is $1, running version $2", "n/a"});
    rep.push_back((IRCReply){003, "RPL_CREATED", "This server was created $1 UTC", "n/a"});
    rep.push_back((IRCReply){004, "RPL_MYINFO", "$1 $2 $3 $4", "n/a"});
    rep.push_back((IRCReply){005, "RPL_BOUNCE", "Try server $1, port $2", "Sent by the server to a user to suggest an alternative server.  This is often used when the connection is refused because the server is already full."});

    //RESERVED NUMERALS
    rep.push_back((IRCReply){209, "RPL_TRACECLASS", "n/a", "n/a"});
    rep.push_back((IRCReply){217, "RPL_STATSQLINE", "n/a", "n/a"});
    rep.push_back((IRCReply){231, "RPL_SERVICEINFO", "n/a", "n/a"});
    rep.push_back((IRCReply){232, "RPL_ENDOFSERVICES", "n/a", "n/a"});
    rep.push_back((IRCReply){233, "RPL_SERVICE", "n/a", "n/a"});
    rep.push_back((IRCReply){234, "RPL_SERVLIST", "n/a", "n/a"});
    rep.push_back((IRCReply){235, "RPL_SERVLISTEND", "n/a", "n/a"});    
    rep.push_back((IRCReply){316, "RPL_WHOISCHANOP", "n/a", "n/a"});
    rep.push_back((IRCReply){361, "RPL_KILLDONE", "n/a", "n/a"});
    rep.push_back((IRCReply){362, "RPL_CLOSING", "n/a", "n/a"});
    rep.push_back((IRCReply){363, "RPL_CLOSEEND", "n/a", "n/a"});
    rep.push_back((IRCReply){373, "RPL_INFOSTART", "n/a", "n/a"});
    rep.push_back((IRCReply){384, "RPL_MYPORTIS", "n/a", "n/a"});
    rep.push_back((IRCReply){466, "ERR_YOUWILLBEBANNED", "n/a", "n/a"});
    rep.push_back((IRCReply){476, "ERR_BADCHANMASK", "n/a", "n/a"});
    rep.push_back((IRCReply){492, "ERR_NOSERVICEHOST", "n/a", "n/a"});

    //COMMAND RESPONSES
    rep.push_back((IRCReply){300, "RPL_NONE", "$1", "Dummy reply number. Not used."});
    rep.push_back((IRCReply){301, "RPL_AWAY", "<nick> :<away message>", "n/a"});
    rep.push_back((IRCReply){302, "RPL_USERHOST", ":[<reply>{<space><reply>}]", "Reply format used by USERHOST to list replies to the query list. The reply string is composed as follows: <reply> ::= <nick>['*'] '=' <'+'|'-'><hostname>. The '*' indicates whether the client has registered as an Operator. The '-' or '+' characters represent whether the client has set an AWAY message or not respectively."});
    rep.push_back((IRCReply){303, "RPL_ISON", ":[<nick> {<space><nick>}]", "Reply format used by ISON to list replies to the query list."});
    rep.push_back((IRCReply){305, "RPL_UNAWAY", ":You are no longer marked as being away", "n/a"});
    rep.push_back((IRCReply){306, "RPL_NOWAWAY", ":You have been marked as being away", "These replies are used with the AWAY command (if allowed). RPL_AWAY is sent to any client sending a PRIVMSG to a client which is away. RPL_AWAY is only sent by the server to which the client is connected. Replies RPL_UNAWAY and RPL_NOWAWAY are sent when the client removes and sets an AWAY message."});
    rep.push_back((IRCReply){311, "RPL_WHOISUSER", "$1 $2 $3 * :$4", "n/a"});
//                                                    <nick> <server> :<server info>
    rep.push_back((IRCReply){312, "RPL_WHOISSERVER", "$1 $2 :$3", "n/a"});
    rep.push_back((IRCReply){313, "RPL_WHOISOPERATOR", "<nick> :is an IRC operator", "n/a"});
    rep.push_back((IRCReply){314, "RPL_WHOWASUSER", "<nick> <user> <host> * :<real name>", "n/a"});
    rep.push_back((IRCReply){317, "RPL_WHOISIDLE", "<nick> <integer> :seconds idle", "n/a"});
    rep.push_back((IRCReply){318, "RPL_ENDOFWHOIS", "<nick> :End of /WHOIS list", "n/a"});
    rep.push_back((IRCReply){319, "RPL_WHOISCHANNELS", "<nick> :{[@|+]<channel><space>}", "Replies 311 - 313, 317 - 319 are all replies generated in response to a WHOIS message. Given that there are enough parameters present, the answering server must either formulate a reply out of the above numerics (if the query nick is found) or return an error reply. The '*' in RPL_WHOISUSER is there as the literal character and not as a wild card. For each reply set, only RPL_WHOISCHANNELS may appear more than once (for long lists of channel names). The '@' and '+' characters next to the channel name indicate whether a client is a channel operator or has been granted permission to speak on a moderated channel. The RPL_ENDOFWHOIS reply is used to mark the end of processing a WHOIS message."});
    rep.push_back((IRCReply){321, "RPL_LISTSTART", "Channel :Users Name", "n/a"});
    rep.push_back((IRCReply){322, "RPL_LIST", "$1 $2 :$3", "n/a"});
    rep.push_back((IRCReply){323, "RPL_LISTEND", ":End of /LIST", "Replies RPL_LISTSTART, RPL_LIST, RPL_LISTEND mark the start, actual replies with data and end of the server's response to a LIST command. If there are no channels available to return, only the start and end reply must be sent."});
    rep.push_back((IRCReply){324, "RPL_CHANNELMODEIS", "<channel> <mode> <mode params>", "n/a"});
    rep.push_back((IRCReply){331, "RPL_NOTOPIC", "$1 :No topic is set", "n/a"});
    rep.push_back((IRCReply){332, "RPL_TOPIC", "$1 :$2", "When sending a TOPIC message to determine the channel topic, one of two replies is sent. If the topic is set, RPL_TOPIC is sent back else RPL_NOTOPIC."});
    rep.push_back((IRCReply){341, "RPL_INVITING", "<channel> <nick>", "Returned by the server to indicate that the attempted INVITE message was successful and is being passed onto the end client."});
    rep.push_back((IRCReply){342, "RPL_SUMMONING", "<user> :Summoning user to IRC", "Returned by a server answering a SUMMON message to indicate that it is summoning that user."});
    rep.push_back((IRCReply){351, "RPL_VERSION", "<version>.<debuglevel> <server> :<comments>", "Reply by the server showing its version details. The <version> is the version of the software being used (including any patchlevel revisions) and the <debuglevel> is used to indicate if the server is running in 'debug mode'. The 'comments' field may contain any comments about the version or further version details."});
    rep.push_back((IRCReply){352, "RPL_WHOREPLY", "<channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>", "n/a"});
    rep.push_back((IRCReply){315, "RPL_ENDOFWHO", "<name> :End of /WHO list", "The RPL_WHOREPLY and RPL_ENDOFWHO pair are used to answer a WHO message. The RPL_WHOREPLY is only sent if there is an appropriate match to the WHO query. If there is a list of parameters supplied with a WHO message, a RPL_ENDOFWHO must be sent after processing each list item with <name> being the item."});
    rep.push_back((IRCReply){353, "RPL_NAMREPLY", "= $1 :$2", "n/a"});
    rep.push_back((IRCReply){366, "RPL_ENDOFNAMES", "$1 :End of /NAMES list", "To reply to a NAMES message, a reply pair consisting of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the server back to the client. If there is no channel found as in the query, then only RPL_ENDOFNAMES is returned. The exception to this is when a NAMES message is sent with no parameters and all visible channels and contents are sent back in a series of RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark the end."});
    rep.push_back((IRCReply){364, "RPL_LINKS", "<mask> <server> :<hopcount> <server info>", "n/a"});
    rep.push_back((IRCReply){365, "RPL_ENDOFLINKS", "<mask> :End of /LINKS list", "In replying to the LINKS message, a server must send replies back using the RPL_LINKS numeric and mark the end of the list using an RPL_ENDOFLINKS reply."});
    rep.push_back((IRCReply){367, "RPL_BANLIST", "<channel> <banid>", "n/a"});
    rep.push_back((IRCReply){368, "RPL_ENDOFBANLIST", "<channel> :End of channel ban list", "When listing the active 'bans' for a given channel, a server is required to send the list back using the RPL_BANLIST and RPL_ENDOFBANLIST messages. A separate RPL_BANLIST is sent for each active banid. After the banids have been listed (or if none present) a RPL_ENDOFBANLIST must be sent."});
    rep.push_back((IRCReply){371, "RPL_INFO", ":$1", "n/a"});
    rep.push_back((IRCReply){374, "RPL_ENDOFINFO", ":End of /INFO list", "A server responding to an INFO message is required to send all its 'info' in a series of RPL_INFO messages with a RPL_ENDOFINFO reply to indicate the end of the replies."});
    rep.push_back((IRCReply){375, "RPL_MOTDSTART", ":- <server> Message of the day - ", "n/a"});
    rep.push_back((IRCReply){372, "RPL_MOTD", ":- <text>", "n/a"});
    rep.push_back((IRCReply){376, "RPL_ENDOFMOTD", ":End of /MOTD command", "When responding to the MOTD message and the MOTD file is found, the file is displayed line by line, with each line no longer than 80 characters, using RPL_MOTD format replies. These should be surrounded by a RPL_MOTDSTART (before the RPL_MOTDs) and an RPL_ENDOFMOTD (after)."});
    rep.push_back((IRCReply){381, "RPL_YOUREOPER", ":You are now an IRC operator", "RPL_YOUREOPER is sent back to a client which has just successfully issued an OPER message and gained operator status."});
    rep.push_back((IRCReply){382, "RPL_REHASHING", "<config file> :Rehashing", "If the REHASH option is used and an operator sends a REHASH message, an RPL_REHASHING is sent back to the operator."});
    rep.push_back((IRCReply){391, "RPL_TIME", "$1 :$2", "When replying to the TIME message, a server must send the reply using the RPL_TIME format above. The string showing the time need only contain the correct day and time there. There is no further requirement for the time string."});
    rep.push_back((IRCReply){392, "RPL_USERSSTART", ":UserID Terminal Host", "n/a"});
    rep.push_back((IRCReply){393, "RPL_USERS", ":%-8s %-9s %-8s", "n/a"});
    rep.push_back((IRCReply){394, "RPL_ENDOFUSERS", ":End of users", "n/a"});
    rep.push_back((IRCReply){395, "RPL_NOUSERS", ":Nobody logged in", "If the USERS message is handled by a server, the replies RPL_USERSTART, RPL_USERS, RPL_ENDOFUSERS and RPL_NOUSERS are used. RPL_USERSSTART must be sent first, following by either a sequence of RPL_USERS or a single RPL_NOUSER. Following this is RPL_ENDOFUSERS."});
    rep.push_back((IRCReply){200, "RPL_TRACELINK", "Link <version & debug level> <destination> <next server>", "n/a"});
    rep.push_back((IRCReply){201, "RPL_TRACECONNECTING", "Try. <class> <server>", "n/a"});
    rep.push_back((IRCReply){202, "RPL_TRACEHANDSHAKE", "H.S. <class> <server>", "n/a"});
    rep.push_back((IRCReply){203, "RPL_TRACEUNKNOWN", "???? <class> [<client IP address in dot form>]", "n/a"});
    rep.push_back((IRCReply){204, "RPL_TRACEOPERATOR", "Oper <class> <nick>", "n/a"});
    rep.push_back((IRCReply){205, "RPL_TRACEUSER", "User <class> <nick>", "n/a"});
    rep.push_back((IRCReply){206, "RPL_TRACESERVER", "Serv <class> <int>S <int>C <server> <nick!user|*!*>@<host|server>", "n/a"});
    rep.push_back((IRCReply){208, "RPL_TRACENEWTYPE", "<newtype> 0 <client name>", "n/a"});
    rep.push_back((IRCReply){261, "RPL_TRACELOG", "File <logfile> <debug level>", "The RPL_TRACE* are all returned by the server in response to the TRACE message. How many are returned is dependent on the the TRACE message and whether it was sent by an operator or not. There is no predefined order for which occurs first. Replies RPL_TRACEUNKNOWN, RPL_TRACECONNECTING and RPL_TRACEHANDSHAKE are all used for connections which have not been fully established and are either unknown, still attempting to connect or in the process of completing the 'server handshake'. RPL_TRACELINK is sent by any server which handles a TRACE message and has to pass it on to another server. The list of RPL_TRACELINKs sent in response to a TRACE command traversing the IRC network should reflect the actual connectivity of the servers themselves along that path. RPL_TRACENEWTYPE is to be used for any connection which does not fit in the other categories but is being displayed anyway."});
    rep.push_back((IRCReply){211, "RPL_STATSLINKINFO", "<linkname> <sendq> <sent messages> <sent bytes> <received messages> <received bytes> <time open>", "n/a"});
    rep.push_back((IRCReply){212, "RPL_STATSCOMMANDS", "<command> <count>", "n/a"});
    rep.push_back((IRCReply){213, "RPL_STATSCLINE", "C <host> * <name> <port> <class>", "n/a"});
    rep.push_back((IRCReply){214, "RPL_STATSNLINE", "N <host> * <name> <port> <class>", "n/a"});
    rep.push_back((IRCReply){215, "RPL_STATSILINE", "I <host> * <host> <port> <class>", "n/a"});
    rep.push_back((IRCReply){216, "RPL_STATSKLINE", "K <host> * <username> <port> <class>", "n/a"});
    rep.push_back((IRCReply){218, "RPL_STATSYLINE", "Y <class> <ping frequency> <connect frequency> <max sendq>", "n/a"});
    rep.push_back((IRCReply){219, "RPL_ENDOFSTATS", "<stats letter> :End of /STATS report", "n/a"});
    

    //ERROR REPLIES
    rep.push_back((IRCReply){401, "ERR_NOSUCHNICK", "$1 :No such nick/channel", "Used to indicate the nickname parameter supplied to a command is currently unused."});
    rep.push_back((IRCReply){402, "ERR_NOSUCHSERVER", "<server name> :No such server", "Used to indicate the server name given currently doesn't exist."});
    rep.push_back((IRCReply){403, "ERR_NOSUCHCHANNEL", "$1 :No such channel", "Used to indicate the given channel name is invalid."});
    rep.push_back((IRCReply){404, "ERR_CANNOTSENDTOCHAN", "<channel name> :Cannot send to channel", "Sent to a user who is either (a) not on a channel which is mode +n or (b) not a chanop (or mode +v) on a channel which has mode +m set and is trying to send a PRIVMSG message to that channel."});
    rep.push_back((IRCReply){405, "ERR_TOOMANYCHANNELS", "<channel name> :You have joined too many channels", "Sent to a user when they have joined the maximum number of allowed channels and they try to join another channel."});
    rep.push_back((IRCReply){406, "ERR_WASNOSUCHNICK", "<nickname> :There was no such nickname", "Returned by WHOWAS to indicate there is no history information for that nickname."});
    rep.push_back((IRCReply){407, "ERR_TOOMANYTARGETS", "<target> :Duplicate recipients. No message delivered", "Returned to a client which is attempting to send PRIVMSG/NOTICE using the user@host destination format and for a user@host which has several occurrences."});
    rep.push_back((IRCReply){409, "ERR_NOORIGIN", ":No origin specified", "PING or PONG message missing the originator parameter which is required since these commands must work without valid prefixes."});
    rep.push_back((IRCReply){411, "ERR_NORECIPIENT", ":No recipient given (<command>)", "n/a"});
    rep.push_back((IRCReply){412, "ERR_NOTEXTTOSEND", ":No text to send", "412 - 414 are returned by PRIVMSG to indicate that the message wasn't delivered for some reason."});
    rep.push_back((IRCReply){413, "ERR_NOTTOPLEVEL", "<mask> :No toplevel domain specified", "412 - 414 are returned by PRIVMSG to indicate that the message wasn't delivered for some reason. ERR_NOTOPLEVEL and ERR_WILDTOPLEVEL are errors that are returned when an invalid use of \"PRIVMSG $<server>\" or \"PRIVMSG #<host>\" is attempted."});
    rep.push_back((IRCReply){414, "ERR_WILDTOPLEVEL", "<mask> :Wildcard in toplevel domain", "412 - 414 are returned by PRIVMSG to indicate that the message wasn't delivered for some reason. ERR_NOTOPLEVEL and ERR_WILDTOPLEVEL are errors that are returned when an invalid use of \"PRIVMSG $<server>\" or \"PRIVMSG #<host>\" is attempted."});
    rep.push_back((IRCReply){421, "ERR_UNKNOWNCOMMAND", "$1 :Unknown command", "Returned to a registered client to indicate that the command sent is unknown by the server."});
    rep.push_back((IRCReply){422, "ERR_NOMOTD", ":MOTD File is missing", "Server's MOTD file could not be opened by the server."});
    rep.push_back((IRCReply){423, "ERR_NOADMININFO", "<server> :No administrative info available", "Returned by a server in response to an ADMIN message when there is an error in finding the appropriate information."});
    rep.push_back((IRCReply){424, "ERR_FILEERROR", ":File error doing <file op> on <file>", "Generic error message used to report a failed file operation during the processing of a message."});
    rep.push_back((IRCReply){431, "ERR_NONICKNAMEGIVEN", ":No nickname given", "Returned when a nickname parameter expected for a command and isn't found."});
    rep.push_back((IRCReply){432, "ERR_ERRONEUSNICKNAME", "$1 :Erroneus nickname", "Returned after receiving a NICK message which contains characters which do not fall in the defined set. See section x.x.x for details on valid nicknames."});
    rep.push_back((IRCReply){433, "ERR_NICKNAMEINUSE", "$1 :Nickname is already in use", "Returned when a NICK message is processed that results in an attempt to change to a currently existing nickname."});
    rep.push_back((IRCReply){436, "ERR_NICKCOLLISION", "<nick> :Nickname collision KILL", "Returned by a server to a client when it detects a nickname collision (registered of a NICK that already exists by another server)."});
    rep.push_back((IRCReply){441, "ERR_USERNOTINCHANNEL", "<nick> <channel> :They aren't on that channel", "Returned by the server to indicate that the target user of the command is not on the given channel."});
    rep.push_back((IRCReply){442, "ERR_NOTONCHANNEL", "<channel> :You're not on that channel", "Returned by the server whenever a client tries to perform a channel effecting command for which the client isn't a member."});
    rep.push_back((IRCReply){443, "ERR_USERONCHANNEL", "<user> <channel> :is already on channel", "Returned when a client tries to invite a user to a channel they are already on."});
    rep.push_back((IRCReply){444, "ERR_NOLOGIN", "<user> :User not logged in", "Returned by the summon after a SUMMON command for a user was unable to be performed since they were not logged in."});
    rep.push_back((IRCReply){445, "ERR_SUMMONDISABLED", ":SUMMON has been disabled", "Returned as a response to the SUMMON command. Must be returned by any server which does not implement it."});
    rep.push_back((IRCReply){446, "ERR_USERSDISABLED", ":USERS has been disabled", "Returned as a response to the USERS command. Must be returned by any server which does not implement it."});
    rep.push_back((IRCReply){451, "ERR_NOTREGISTERED", ":You have not registered", "Returned by the server to indicate that the client must be registered before the server will allow it to be parsed in detail."});
    rep.push_back((IRCReply){461, "ERR_NEEDMOREPARAMS", "$1 :Not enough parameters", "Returned by the server by numerous commands to indicate to the client that it didn't supply enough parameters."});
    rep.push_back((IRCReply){462, "ERR_ALREADYREGISTRED", ":You may not reregister", "Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message)."});
    rep.push_back((IRCReply){463, "ERR_NOPERMFORHOST", ":Your host isn't among the privileged", "Returned to a client which attempts to register with a server which does not been setup to allow connections from the host the attempted connection is tried."});
    rep.push_back((IRCReply){464, "ERR_PASSWDMISMATCH", ":Password incorrect", "Returned to indicate a failed attempt at registering a connection for which a password was required and was either not given or incorrect."});
    rep.push_back((IRCReply){465, "ERR_YOUREBANNEDCREEP", ":You are banned from this server", "Returned after an attempt to connect and register yourself with a server which has been setup to explicitly deny connections to you."});
    rep.push_back((IRCReply){467, "ERR_KEYSET", "<channel> :Channel key already set", "n/a"});
    rep.push_back((IRCReply){471, "ERR_CHANNELISFULL", "<channel> :Cannot join channel (+l)", "n/a"});
    rep.push_back((IRCReply){472, "ERR_UNKNOWNMODE", "<char> :is unknown mode char to me", "n/a"});
    rep.push_back((IRCReply){473, "ERR_INVITEONLYCHAN", "<channel> :Cannot join channel (+i)", "n/a"});
    rep.push_back((IRCReply){474, "ERR_BANNEDFROMCHAN", "<channel> :Cannot join channel (+b)", "n/a"});
    rep.push_back((IRCReply){475, "ERR_BADCHANNELKEY", "$1 :Cannot join channel (+k)", "n/a"});
    rep.push_back((IRCReply){481, "ERR_NOPRIVILEGES", ":Permission Denied- You're not an IRC operator", "Any command requiring operator privileges to operate must return this error to indicate the attempt was unsuccessful."});
    rep.push_back((IRCReply){482, "ERR_CHANOPRIVSNEEDED", "<channel> :You're not channel operator", "Any command requiring 'chanop' privileges (such as MODE messages) must return this error if the client making the attempt is not a chanop on the specified channel."});
    rep.push_back((IRCReply){483, "ERR_CANTKILLSERVER", ":You cant kill a server!", "Any attempts to use the KILL command on a server are to be refused and this error returned directly to the client."});
    rep.push_back((IRCReply){491, "ERR_NOOPERHOST", ":No O-lines for your host", "If a client sends an OPER message and the server has not been configured to allow connections from the client's host as an operator, this error must be returned."});
    rep.push_back((IRCReply){501, "ERR_UMODEUNKNOWNFLAG", ":Unknown MODE flag", "Returned by the server to indicate that a MODE message was sent with a nickname parameter and that the a mode flag sent was not recognized."});
    rep.push_back((IRCReply){502, "ERR_USERSDONTMATCH", ":Cant change mode for other users", "Error sent to any user trying to view or change the user mode for a user other than themselves."});
    rep.push_back((IRCReply){241, "RPL_STATSLLINE", "L <hostmask> * <servername> <maxdepth>", "n/a"});
    rep.push_back((IRCReply){242, "RPL_STATSUPTIME", ":Server Up %d days %d:%02d:%02d", "n/a"});
    rep.push_back((IRCReply){243, "RPL_STATSOLINE", "O <hostmask> * <name>", "n/a"});
    rep.push_back((IRCReply){244, "RPL_STATSHLINE", "H <hostmask> * <servername>", "n/a"});
    rep.push_back((IRCReply){221, "RPL_UMODEIS", "<user mode string>", "To answer a query about a client's own mode, RPL_UMODEIS is sent back."});
    rep.push_back((IRCReply){251, "RPL_LUSERCLIENT", ":There are <integer> users and <integer> invisible on <integer> servers", "n/a"});
    rep.push_back((IRCReply){252, "RPL_LUSEROP", "<integer> :operator(s) online", "n/a"});
    rep.push_back((IRCReply){253, "RPL_LUSERUNKNOWN", "<integer> :unknown connection(s)", "n/a"});
    rep.push_back((IRCReply){254, "RPL_LUSERCHANNELS", "<integer> :channels formed", "n/a"});
    rep.push_back((IRCReply){255, "RPL_LUSERME", ":I have <integer> clients and <integer> servers", "In processing an LUSERS message, the server sends a set of replies from RPL_LUSERCLIENT, RPL_LUSEROP, RPL_USERUNKNOWN, RPL_LUSERCHANNELS and RPL_LUSERME. When replying, a server must send back RPL_LUSERCLIENT and RPL_LUSERME. The other replies are only sent back if a non-zero count is found for them."});
    rep.push_back((IRCReply){256, "RPL_ADMINME", "<server> :Administrative info", "n/a"});
    rep.push_back((IRCReply){257, "RPL_ADMINLOC1", ":<admin info>", "n/a"});
    rep.push_back((IRCReply){258, "RPL_ADMINLOC2", ":<admin info>", "n/a"});
    rep.push_back((IRCReply){259, "RPL_ADMINEMAIL", ":<admin info>", "When replying to an ADMIN message, a server is expected to use replies RLP_ADMINME through to RPL_ADMINEMAIL and provide a text message with each. For RPL_ADMINLOC1 a description of what city, state and country the server is in is expected, followed by details of the university and department (RPL_ADMINLOC2) and finally the administrative contact for the server (an email address here is required) in RPL_ADMINEMAIL."});
    //Nick omarakoudadmikwad

    for(std::vector<IRCReply>::iterator obj = rep.begin(); obj != rep.end(); obj++)
    {
        if ((*obj).code == code)
            return *obj;
    }

    return ((IRCReply){-1, "", "", ""});
}

void    replace(std::string &line, std::string &needle, std::string &str)
{
    size_t pos = 0;

    if ((pos = line.find(needle, 0)) != std::string::npos)
    {
        line.erase(pos, needle.length());
        line.insert(pos, str);
    }
}

IRCReply    get_replay( int code, std::vector<std::string> vars )
{
    IRCReply rep = replay_list(code);
    std::string needle = "$1";
    
    for(std::vector<std::string>::iterator var = vars.begin(); var != vars.end(); var++)
    {
        replace(rep.msg, needle, *var);
        needle[1] += 1;
    }
    return rep;
}

std::string ft_rep_code(int num) {
    char buf[20];
    int i = 0;

    if (num == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
        return std::string(buf);
    }

    bool is_negative = false;
    if (num < 0) {
        is_negative = true;
        num = -num;
    }

    while (num != 0) {
        int digit = num % 10;
        buf[i++] = digit + '0';
        num /= 10;
    }

    if (is_negative) {
        buf[i++] = '-';
    }

    buf[i] = '\0';
    std::string str(buf);
    reverse(str.begin(), str.end());
    if (str.length() == 1)
        str = "00" + str;
    if(str.length() == 2)
        str = "0" + str;
    return str;
}

std::string ft_itoa(int num) {
    char buf[20];
    int i = 0;

    if (num == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
        return std::string(buf);
    }

    bool is_negative = false;
    if (num < 0) {
        is_negative = true;
        num = -num;
    }

    while (num != 0) {
        int digit = num % 10;
        buf[i++] = digit + '0';
        num /= 10;
    }

    if (is_negative) {
        buf[i++] = '-';
    }

    buf[i] = '\0';
    std::string str(buf);
    reverse(str.begin(), str.end());
    return str;
}
