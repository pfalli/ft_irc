#ifndef NUMERICMESSAGES_HPP
#define NUMERICMESSAGES_HPP


// **ERRORS
# define ERR_NEEDMOREPARAMS(username) ":serverhost 461 " + username + " :Not enough parameters\r\n"
# define ERR_TOOMANYPARAMS(username) ":serverhost 461 " + username + " :Too many parameters\r\n"
# define ERR_USERNOTINCHANNEL(client, nick, channel) ":" + client + " " + nick + " " + channel + ":Error (441): They aren't on that channel\r\n"
# define ERR_NOTONCHANNEL(client, channel) ":" + client + " " + channel + " Error(442):You're not on that channel\r\n"
# define ERR_USERONCHANNEL(client, nick, channel) ":" + client + " " + nick + " " + channel + "Error(443):is already on channel\r\n"
# define ERR_NOTEXIST(target) ":serverhost 461 " + target + " doesn't exist\r\n"
# define ERR_INVERTPARAM(command, target) ":serverhost 461 '" + target + "' is not a channel. "+command+" <client> <channel> \r\n"
# define ERR_UNKNOWNCOMMAND(username, command) ":serverhost 421 " + username + " " + command + " :Unknown command\r\n"


// **REPLIES
# define RPL_QUIT(nickname, username, reason) ":" + nickname + "!" + username + "@localhost QUIT :Quit: " + reason + "\r\n"
#define RPL_KICK(kickernick, kickeruser, channelname, username, reason) ":" + kickernick + "!" + kickeruser + "@localhost KICK " + channelname + " " + username + ", reason: " + reason + "\r\n"
# define RPL_INVITING(client, username, targetnick, channel) ":" + client + "!" + username +"@localhost  INVITE " + targetnick + " to " + channel + "\r\n"
# define RPL_NOTICE(client, username, targetnick, message) ":" + client + "!" + username + "@localhost NOTICE " + targetnick + " :" + message + "\r\n"

/* PRIVMSG */
# define ERR_NOSUCHNICK(client, nickname) ( std::string(RED) + ":" + client + " " + nickname + ":" + "401 Error: No such nick/channel" + std::string(RESET) + "\r\n" )
# define SUCCESS_PRIVMSG(sender, recipient, msg) (std::string(YELLOW) + ":" + sender + " PRIVMSG " + recipient + " :" + msg +  std::string(RESET) + "\r\n")
# define TO_ALL_CHANNEL(senderNickname, channelName, msg) (":" + senderNickname + " PRIVMSG " + channelName + " :" + msg + "\r\n")
# define ERR_NOSUCHCHANNEL(serverName, nickName, channelName) (std::string(RED) + serverName + ": Error(403)" + nickName + " " + channelName + " :No such channel" + std::string(RESET) + "\r\n")

/* join */
# define ERR_NEEDMOREPARAMS2(userName, command) (std::string (RED) + userName + " " + command + " :Not enough parameters + std::string(RESET)" + "\r\n")
# define JOIN_SUCCESS(joiner, channelName) (std::string(BLUE) + joiner + " is joining the channel " + channelName + std::string(RESET) + "\r\n")
# define RPL_TOPIC(userName, channelName, topic) (std::string(GREEN) + channelName + " :" + topic + std::string(RESET) + "\r\n") 
# define RPL_TOPICWHOTIME(userName, channelName, nickName, time) (std::string(GREEN) + userName + " " + channelName + " " + nickName + " " + timeToString(time) + std::string(RESET) + "\r\n")
# define RPL_NAMREPLY(nickName, channelName, channel) (nickName + "=" + channelName + " :" + ((channel).makeMemberList()) + "\r\n")
# define RPL_ENDOFNAMES(nickName, channelName) (nickName + " " + channelName + " :End of /NAMES list \r\n")
# define ERR_INVITEONLYCHAN(serverName, nickName, channelName)(std::string(RED) + serverName + ": Error(473) " + nickName + " " + channelName + " :Cannot join channel (+i)" + std::string(RESET) + "\r\n")
/* TOPIC */
# define RPL_NOTOPIC(userName, channelName) (std::string(GREEN) + userName + " " + channelName + " :No Topic is set" + std::string(RESET) + "\r\n")

/* MODE */
# define RPL_CHANNELMODEIS(serverName, nickName, channelName, modeString) (std::string(BLUE) + ":" + serverName + " 324 " + nickName + " " + channelName + " " + modeString + std::string(RESET) + "\r\n")
# define RPL_CREATIONTIME(serverName, nickName, channelName, creationTime) (std::string(BLUE) + ":" + serverName + " 329 " + nickName + " " + channelName + " " + timeToString(creationTime) + std::string(RESET) + "\r\n")
# define ERR_UMODEUNKNOWNFLAG(serverName, nickName) (std::string(RED) + ":" + serverName + " Error(501) " + nickName + " :unknown MODE flag" + std::string(RESET) + "\r\n")
# define ERR_UNKNOWNMODE(serverName, nickName, char) (std::string(RED) + ":" + serverName + " Error(472) " + nickName + " " + char + " :is unknown mode char to me" + std::string(RESET) + "\r\n") 
# define ERR_CHANOPRIVSNEEDED(serverName, nickName, channelName) (std::string(RED) + ":" + serverName + " Error(485) " + nickName + " " + channelName + " :You're not channel operator"+ std::string(RESET) + "\r\n")

# define WELCOME_MESSAGE "\n\
	Hello, welcome to out server!\n\
	To properly use our server you have to register.\n\
	Please use the following commands in order :)\n\n\
	PASS <password> to register with the server password\n\
	USER <username> to set Username\n\
	NICK <nickname> to set Nickname\n\n\
	If you want to know what commands you can use just type HELP\n\n";


#endif