#ifndef NUMERICMESSAGES_HPP
#define NUMERICMESSAGES_HPP


// **ERRORS
# define ERR_NEEDMOREPARAMS(username) ":serverhost 461 " + username + " :Not enough parameters\r\n"
# define ERR_TOOMANYPARAMS(username) ":serverhost 461 " + username + " :Too many parameters\r\n"
# define ERR_USERNOTINCHANNEL(client, nick, channel) ":" + client + " " + nick + " " + channel + ":Error (441): They aren't on that channel\r\n"
# define ERR_NOTONCHANNEL(client, channel) ":" + client + " " + channel + " Error(442):You're not on that channel\r\n"
# define ERR_USERONCHANNEL(client, nick, channel) ":" + client + " " + nick + " #" + channel + "Error(443):is already on channel\r\n"
# define ERR_NOTEXIST(target) ":serverhost 461 " + target + " doesn't exist\r\n"


// **REPLIES
# define RPL_QUIT(nickname, username, reason) ":" + nickname + "!" + username + "@localhost QUIT :Quit: " + reason + "\r\n"
#define RPL_KICK(kickernick, kickeruser, channelname, username) ":" + kickernick + "!" + kickeruser + "@localhost KICK #" + channelname + " " + username + "\r\n"
# define RPL_INVITING(client, username, targetnick, channel) ":" + client + "!" + username +"@localhost  INVITE " + targetnick + " to #" + channel + "\r\n"

/* PRIVMSG */
# define ERR_NOSUCHNICK(client, nickname) ( std::string(RED) + ":" + client + " " + nickname + ":" + "401 Error: No such nick/channel" + std::string(RESET) + "\r\n" )
# define SUCCESS_PRIVMSG(sender, recipient, msg) (std::string(YELLOW) + ":" + sender + " PRIVMSG " + recipient + " :" + msg +  std::string(RESET) + "\r\n")
# define TO_ALL_CHANNEL(senderNickname, channelName, msg) (":" + senderNickname + " PRIVMSG " + channelName + " :" + msg + "\r\n")
# define ERR_NOSUCHCHANNEL(userName, channelName) (std::string(RED) + userName + " " + channelName + " :403Error: No such channel" + std::string(RESET) + "\r\n")

/* join */
# define ERR_NEEDMOREPARAMS2(userName, command) (std::string (RED) + userName + " " + command + " :Not enough parameters + std::string(RESET)" + "\r\n")
# define JOIN_SUCCESS(joiner, channelName) (std::string(BLUE) + joiner + " is joining the channel " + channelName + std::string(RESET) + "\r\n")
# define RPL_TOPIC(userName, channelName, topic) (std::string(GREEN) + channelName + " :" + topic + std::string(RESET) + "\r\n") 
# define RPL_TOPICWHOTIME(userName, channelName, nickName, time) (std::string(GREEN) + userName + " " + channelName + " " + nickName + " " + timeToString(time) + std::string(RESET) + "\r\n")
# define RPL_NAMREPLY(nickName, channelName, channel) (nickName + "=" + channelName + " :" + ((channel).makeMemberList()) + "\r\n")
# define RPL_ENDOFNAMES(nickName, channelName) (nickName + " " + channelName + " :End of /NAMES list \r\n")

/* TOPIC */
# define RPL_NOTOPIC(userName, channelName) (std::string(GREEN) + userName + " " + channelName + " :No Topic is set" + std::string(RESET) + "\r\n")

/* MODE */
# define RPL_CHANNELMODEIS(serverName, nickName, channelName, modeString, parameter) (std::string(BLUE) + ": " + serverName + " 324 " + nickName + " " + channelName + " " + modeString + " " + parameter + std::string(RESET) + "\r\n")
# define RPL_CREATIONTIME(serverName, nickName, channelName, creationTime) (std::string(BLUE) + ": " + serverName + " 329 " + nickName + " " + channelName + " " + timeToString(creationTime) + std::string(RESET) + "\r\n")

# define WELCOME_MESSAGE "Welcome to our server\nTo register, please use the following commands:\nPASS <password>\nUSER <username>\nNICK <nickname>\n"


#endif