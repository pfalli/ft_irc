#ifndef NUMERICMESSAGES_HPP
#define NUMERICMESSAGES_HPP

// **ERRORS
# define ERR_NEEDMOREPARAMS(username) ":serverhost 461 " + username + " :Not enough parameters\r\n"
# define ERR_TOOMANYPARAMS(username) ":serverhost 461 " + username + " :Too many parameters\r\n"


// **REPLIES
# define RPL_QUIT(nickname, username, reason) ":" + nickname + "!" + username + "@localhost QUIT :Quit: " + reason + "\r\n"

/* PRIVMSG */
# define ERR_NOSUCHNICK(client, nickname) (std::string(RED) + ":" + client + " " + nickname + ":" + "401 Error: No such nick/channel\r\n" + std::string(RESET))
# define SUCCESS_PRIVMSG(sender, recipient, msg) (std::string(YELLOW) + ":" + sender + " PRIVMSG " + recipient + " :" + msg + "\r\n" + std::string(RESET))
# define TO_ALL_CHANNEL(senderNickname, channelName, msg) (": " + senderNickname + " PRIVMSG #" + channelName + " :" + msg + "\r\n")
# define ERR_NOSUCHCHANNEL(userName, channelName) (std::string(RED) + userName + " " + channelName + " :403Error: No such channel\r\n" + std::string(RESET))

# define WELCOME_MESSAGE "Welcome to our server\nTo register, please use the following commands:\nPASS <password>\nUSER <username>\nNICK <nickname>\n"

#endif