#ifndef NUMERICMESSAGES_HPP
#define NUMERICMESSAGES_HPP


// **ERRORS
# define ERR_NEEDMOREPARAMS(username) ":serverhost 461 " + username + " :Not enough parameters\r\n"
# define ERR_TOOMANYPARAMS(username) ":serverhost 461 " + username + " :Too many parameters\r\n"
#define ERR_USERNOTINCHANNEL(client, nick, channel) ":" + client + " " + nick + " " + channel + ":Error (441): They aren't on that channel\r\n"


// **REPLIES
# define RPL_QUIT(nickname, username, reason) ":" + nickname + "!" + username + "@localhost QUIT :Quit: " + reason + "\r\n"
#define RPL_KICK(kickernick, kickeruser, channelname, username) ":" + kickernick + "!" + kickeruser + "@localhost KICK #" + channelname + " " + username + "\r\n"

/* PRIVMSG */
# define ERR_NOSUCHNICK(client, nickname) (std::string(RED) + ":" + client + " " + nickname + ":" + "401 Error: No such nick/channel\r\n" + std::string(RESET))
# define SUCCESS_PRIVMSG(sender, recipient, msg) (std::string(YELLOW) + ":" + sender + " PRIVMSG " + recipient + " :" + msg + "\r\n" + std::string(RESET))
# define TO_ALL_CHANNEL(senderNickname, channelName, msg) (":" + senderNickname + " PRIVMSG #" + channelName + " :" + msg + "\r\n")
# define ERR_NOSUCHCHANNEL(userName, channelName) (std::string(RED) + userName + " " + channelName + " :403Error: No such channel\r\n" + std::string(RESET))

/* join */
# define ERR_NEEDMOREPARAMS2(userName, command) (std::string (RED) + userName + " " + command " :Not enough parameters\r\n" + std::string(RESET))
# define JOIN_SUCCESS(joiner, channelName) (std::string(BLUE) + joiner + " is joining the channel #" + channelName + "\r\n" + std::string(RESET))
# define RPL_TOPIC(userName, channelName, topic) (std::string(GREEN) + " " + channelName + " :" + topic + "\r\n" + std::string(RESET)) 
# define RPL_TOPICWHOTIME(userName, channelName, nickName, time) (std::string(GREEN) + userName + " " + channelName + " " + nickName + " " + timeToString(time) + "\r\n" + std::string(RESET))
# define RPL_NAMREPLY(nickName, channelName, channel) (nickName + "=" + channelName + " :" + ((channel).makeMemberList()) + "\r\n")
# define RPL_ENDOFNAMES(nickName, channelName) (nickName + " " + channelName + " :End of /NAMES list") 

# define WELCOME_MESSAGE "Welcome to our server\nTo register, please use the following commands:\nPASS <password>\nUSER <username>\nNICK <nickname>\n"


#endif