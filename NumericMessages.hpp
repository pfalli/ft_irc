#ifndef NUMERICMESSAGES_HPP
#define NUMERICMESSAGES_HPP

// **ERRORS
# define ERR_NEEDMOREPARAMS(username) ":serverhost 461 " + username + " :Not enough parameters\r\n"


// **REPLIES
# define RPL_QUIT(nickname, username, reason) ":" + nickname + "!" + username + "@localhost QUIT :Quit: " + reason + "\r\n"

#endif