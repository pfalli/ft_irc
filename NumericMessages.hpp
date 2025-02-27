#ifndef NUMERICMESSAGES_HPP
#define NUMERICMESSAGES_HPP

// **ERRORS
# define ERR_NEEDMOREPARAMS(username) ":serverhost 461 " + username + " :Not enough parameters\r\n"
# define ERR_TOOMANYPARAMS(username) ":serverhost 461 " + username + " :Too many parameters\r\n"


// **REPLIES
# define RPL_QUIT(nickname, username, reason) ":" + nickname + "!" + username + "@localhost QUIT :Quit: " + reason + "\r\n"

# define WELCOME_MESSAGE "Welcome to our server\nTo register, please use the following commands:\nPASS <password>\nUSER <username>\nNICK <nickname>\n"
#endif