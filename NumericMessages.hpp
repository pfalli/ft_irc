#ifndef NUMERICMESSAGES_HPP
#define NUMERICMESSAGES_HPP

// **ERRORS
# define ERR_NEEDMOREPARAMS(username) ":serverhost 461 " + username + " :Not enough parameters\r\n"
# define ERR_TOOMANYPARAMS(username) ":serverhost 461 " + username + " :Too many parameters\r\n"
# define ERR_USERNOTINCHANNEL(client, nick, channel) ":" + client + " " + nick + " " + channel + ":Error (441): They aren't on that channel\r\n"
# define ERR_NOTONCHANNEL(client, channel) ":" + client + " #"+ channel + " Error(442):You're not on that channel\r\n"
# define ERR_USERONCHANNEL(client, nick, channel) ":" + client + " " + nick + " #" + channel + "Error(443):is already on channel\r\n"


// **REPLIES
# define RPL_QUIT(nickname, username, reason) ":" + nickname + "!" + username + "@localhost QUIT :Quit: " + reason + "\r\n"
#define RPL_KICK(kickernick, kickeruser, channelname, username) ":" + kickernick + "!" + kickeruser + "@localhost KICK #" + channelname + " " + username + "\r\n"
# define RPL_INVITING(client, username, targetnick, channel) ":" + client + "!" + username +"@localhost  INVITE " + targetnick + " to #" + channel + "\r\n"

/* PRIVMSG */
# define ERR_NOSUCHNICK(client, nickname) (std::string(RED) + ":" + client + " " + nickname + ":" + "401 Error: No such nick/channel\r\n" + std::string(RESET))
# define SUCCESS_PRIVMSG(sender, recipient, msg) (std::string(YELLOW) + ":" + sender + " PRIVMSG " + recipient + " :" + msg + "\r\n" + std::string(RESET))
# define TO_ALL_CHANNEL(senderNickname, channelName, msg) (": " + senderNickname + " PRIVMSG #" + channelName + " :" + msg + "\r\n")
# define ERR_NOSUCHCHANNEL(userName, channelName) (std::string(RED) + userName + " " + channelName + " :403Error: No such channel\r\n" + std::string(RESET))

# define WELCOME_MESSAGE "Welcome to our server\nTo register, please use the following commands:\nPASS <password>\nUSER <username>\nNICK <nickname>\n"

#endif