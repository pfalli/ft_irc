#ifndef NUMERICMESSAGES_HPP
#define NUMERICMESSAGES_HPP


// **ERRORS
# define ERR_NEEDMOREPARAMS(username) ":serverhost 461 " + username + " :Not enough parameters\r\n"
# define ERR_TOOMANYPARAMS(username) ":serverhost 461 " + username + " :Too many parameters\r\n"
# define ERR_USERNOTINCHANNEL(client, nick, channel) ":" + client + " " + nick + " " + channel + ":Error (441): They aren't on that channel\r\n"
# define ERR_NOTONCHANNEL(client, channel) ":serverhost 442 " + client + " " + channel + " :You're not on that channel\r\n"
# define ERR_USERONCHANNEL(client, nick, channel) ":serverhost 443 " + client + " " + channel + " " + nick + ":is already on channel\r\n"
# define ERR_NOTEXIST(target) ":serverhost 461 " + target + " doesn't exist\r\n"
# define ERR_INVERTPARAM(command, target) ":serverhost 461 '" + target + "' is not a channel. "+command+" <client> <channel> \r\n"
# define ERR_UNKNOWNCOMMAND(username, command) ":serverhost 421 " + username + " " + command + " :Unknown command\r\n";
# define ERR_USERONCHANNEL2(clientName, target, channelName) ":serverhost 443 " + clientName + " " + target + " " + channelName + " :is already on channel\r\n"
# define ERR_PASSWORDWRONG(Username) ":serverhost 462 " + Username + " :Password incorrect\r\n";
# define ERR_ALREADYREGISTERED(username) ":serverhost 462 " + username + " :You may not reregister\r\n";
# define ERR_NOTREGISTERED ":serverhost 451 new_client :You have not registered";
# define ERR_NICKNAMEINUSE(nickname) ":serverhost 433 " + nickname + " :Nickname is already in use\r\n";
# define ERR_ERRONEUSNICKNAME(username, nickname) ":serverhost 432 " + username + " " + nickname + " :Erroneus nickname";
// **REPLIES
# define RPL_QUIT(username, reason) "" + username + " /Quit reason: " + reason + "\r\n"
#define RPL_KICK(kickernick, kickeruser, channelname, username, reason) ":" + kickernick + "!" + kickeruser + "@serverhost KICK " + channelname + " " + username + " :" + reason + "\r\n"
# define RPL_INVITING(client, targetnick, channel) ":serverhost 341 " + client + targetnick + channel + "\r\n"
# define RPL_NOTICE(client, username, targetnick, message) ":" + client + "!" + username + "@localhost NOTICE " + targetnick + " :" + message + "\r\n"

/* PRIVMSG */
# define ERR_NOSUCHNICK(client, nickname) ( ":serverhost 401" + client + " " + nickname + ":" + " : No such nick/channel"  + "\r\n" )
# define SUCCESS_PRIVMSG(sender, recipient, msg)  ":" + sender + " PRIVMSG " + recipient + " :" + msg  + "\r\n"
# define TO_ALL_CHANNEL(senderNickname, channelName, msg) (":" + senderNickname + " PRIVMSG " + channelName + " :" + msg + "\r\n")
# define ERR_NOSUCHCHANNEL(nickName, channelName) ( ":serverhost 403 " + nickName + " " + channelName + " :No such channel" + "\r\n") // ok
# define TO_ALL_CHANNEL_TEST(senderNick, recipient, msg, senderUser, serverName)  (senderNick + "!" + senderUser + "@" + serverName + " PRIVMSG " + recipient + " :" + msg  + "\r\n")

/* join */
// # define ERR_NEEDMOREPARAMS2(serverName, userName, command) (":" + serverName + " 461 " + userName + " " + command + " :Not enough parameters" + "\r\n")
# define JOIN_SUCCESS(nickName, channelName, serverName, userName) ( ":" + nickName + "!" + userName + "@" + serverName + " JOIN " + channelName + "\r\n")
# define RPL_TOPIC(nickName, channelName, topic) (":serverhost 332 " +nickName + " " + channelName + " :" + topic + "\r\n") 
# define RPL_NAMREPLY(nickName, channelName, channel) (nickName + "=" + channelName + " :" + ((channel).makeMemberList()) + "\r\n")
# define RPL_ENDOFNAMES(nickName, channelName) (nickName + " " + channelName + " :End of /NAMES list \r\n")

# define RPL_TOPICWHOTIME(channelName, nickName, setterNickname, time) (":serverhost 333 " +  nickName + " " + channelName + " " + setterNickname + " " + timeToString(time) + "\r\n")
# define ERR_INVITEONLYCHAN(serverName, nickName, channelName)(  serverName + ": Error(473) " + nickName + " " + channelName + " :Cannot join channel (+i)" + "\r\n")
# define ERR_BADCHANNELKEY(serverName, nickName, channelName)(  serverName + ": Error(475) " + nickName + " " + channelName + " :Cannot join channel (+k)" + "\r\n")
/* TOPIC */
# define RPL_NOTOPIC(nickName, channelName) ("serverhost 331 " + nickName + " " + channelName + " :No Topic is set" + "\r\n")
# define TOPIC_SET(nickName, userName, channelName, topic) (":" + nickName + "!" + userName + "@" + "serverhost" + " TOPIC " + channelName + " :" + topic + "\r\n")

/* MODE */
# define RPL_CHANNELMODEIS(serverName, nickName, channelName, modeString) (  ":" + serverName + " 324 " + nickName + " " + channelName + " " + modeString + "\r\n")
# define RPL_CREATIONTIME(serverName, nickName, channelName, creationTime) (  ":" + serverName + " 329 " + nickName + " " + channelName + " " + timeToString(creationTime) + "\r\n")
# define ERR_UMODEUNKNOWNFLAG(serverName, nickName) (  ":" + serverName + " Error(501) " + nickName + " :unknown MODE flag" + "\r\n")
# define ERR_UNKNOWNMODE(nickName, char) (  ":serverhost 472 " + nickName + " " + char + " :is unknown mode char to me" + "\r\n") 
# define ERR_CHANOPRIVSNEEDED(nickName, channelName) (  ":serverhost 482 " + nickName + " " + channelName + " :You're not channel operator "+ "\r\n")

# define WELCOME_MESSAGE "\n\
	Hello, welcome to our server!\n\
	To properly use our server you have to register.\n\
	Please use the following commands in order :)\n\n\
	PASS <password> to register with the server password\n\
	USER <username> 0 * <real name> to set Username and your real name\n\
	NICK <nickname> to set Nickname\n\n\
	If you want to know what commands you can use just type HELP\n\n";


#endif