#include "Server.hpp"
#include "utils.hpp"
#include "Channel.hpp"
#include "NumericMessages.hpp"

void Server::handleCommand(const Command &cmd, Client &client) {
	if (cmd.command == "HELP")
		giveHelp(client);
	else if (cmd.command == "PASS")
		_register(client, cmd, PASSWORD);
	else if (cmd.command == "USER")
		_register(client, cmd, USERNAME);
	else if (cmd.command == "NICK")
		_register(client, cmd, NICKNAME);
	if (!client.getRegistered())
		return ;
	if (cmd.command == "JOIN") {
		join(this, &client, cmd.parameter);
	}
	else if (cmd.command == "MODE") {
		mode(this, cmd, client);
	} else if (cmd.command == "KICK") {
		handleKick(&client, cmd);
	} else if (cmd.command == "PRIVMSG") {
		privmsg(this, &client, cmd);
	} else if (cmd.command == "QUIT") {
		handleQuit(&client, cmd);
	}
	else if (cmd.command == "PING") {
		handlePing(&client, cmd);
	}
	else if (cmd.command == "PONG") {
		handlePong(&client);
	}
	else if (cmd.command == "KICK") {
		handleKick(&client, cmd);
	}
	if (cmd.command == "TOPIC") {
		topic(this, cmd, client);
	}
    else if (cmd.command == "INVITE") {
		handleInvite(&client, cmd);
	}
	else if (cmd.command == "INFO") {
		printInfo(&client, cmd);
	}
	else if (cmd.command == "NOTICE") {
		handleNotice(&client, cmd);
	}
	else
	{}
}

void Server::giveHelp(const Client &client)
{
	std::string message;
	message =	"Commands: (the first three you have to do in that oder)\n\
	PASS <password> to register with the server password\n\
	USER <username> to set Username\n\
	NICK <nickname> to set Nickname\n\n\
	To use the following you have to register first!\n\
	Give the password, your username and nickname first.\n\n\
	MODE <channel name> <flag>\n\
	KICK #<channel name> <nickname of member> to kick someone from a channel\n\
	PRIVMSG <nickname> to start a private chat\n\
	TOPIC <channel to set the topic of a channel\n\
	QUIT to leave our server\n";
	send(client.getSocket(), message.c_str(), strlen(message.c_str()), 0);
}

void Server::handleNotice(Client *handleClient, const Command &cmd) {
	std::istringstream iss(cmd.parameter);
	std::string target;
	iss >> target;
	if (target.empty() || cmd.message.empty()) {
		std::string str = ERR_NEEDMOREPARAMS(handleClient->getUserName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}	
	if (target[0] == '#') {
		// Target is a channel
		Channel *channel = isChannelExist2(target);
		if (channel == NULL) {
			std::string str = ERR_NOSUCHCHANNEL(this->name, handleClient->getUserName(), target);
			send(handleClient->getSocket(), str.c_str(), str.length(), 0);
			return;
		}
		if (channel->isUserInChannel(handleClient->getNickName()) == NULL) {
			std::string str = ERR_NOTONCHANNEL(handleClient->getNickName(), target);
			send(handleClient->getSocket(), str.c_str(), str.length(), 0);
			return;
		}
		std::string str = RPL_NOTICE(handleClient->getNickName(), handleClient->getUserName(), target, cmd.message);
		sendToChannel(*channel, str);
	} else {
		// Target exist?
		std::vector<Client>::iterator targetIt = clients.begin();
		while (targetIt != clients.end()) {
			if (targetIt->getNickName() == target) {
				break;
			}
			targetIt++;
		}
		if (targetIt == clients.end()) {
			std::string str = ERR_NOSUCHNICK(handleClient->getNickName(), target);
			send(handleClient->getSocket(), str.c_str(), str.length(), 0);
			return;
		}
		std::string str = RPL_NOTICE(handleClient->getNickName(), handleClient->getUserName(), target, cmd.message);
		send(targetIt->getSocket(), str.c_str(), str.length(), 0);
	}
}

// **DEBUGGER PRINT CHANNEL INFO BY NAME***
void Server::printInfo(Client* handleClient, const Command &cmd) {
	std::vector<Channel>::iterator channelIt = channels.begin();
	while (channelIt != channels.end()) {
		if (channelIt->getName() == cmd.parameter) {
			break;
		}
		channelIt++;
	}
	if (channelIt == channels.end()) {
		send(handleClient->getSocket(), "Channel not found\n", 19, 0);
		return ;
	}
	// print _joinedClients
	std::vector<Client *>::iterator listIt = channelIt->getJoinedClients().begin();
	std::string clientList = "Clients in channel " + cmd.parameter + ":\n";
	while (listIt != channelIt->getJoinedClients().end()) {
		clientList += " => " + (*listIt)->getNickName() + "\n";
		listIt++;
	}
	send(handleClient->getSocket(), clientList.c_str(), clientList.length(), 0);
	// print _operators
	std::vector<Client *>::iterator opIt = channelIt->getOperators().begin();
	std::string opList = "Operators in channel " + cmd.parameter + ":\n";
	while (opIt != channelIt->getOperators().end()) {
		opList += " => " + (*opIt)->getNickName() + "\n";
		opIt++;
	}
	send(handleClient->getSocket(), opList.c_str(), opList.length(), 0);

}

// INVITE <nickname> <channel>
void Server::handleInvite(Client* handleClient, const Command &cmd) {
	std::istringstream iss(cmd.parameter);
	std::string channelName, targetNick;
	iss >> targetNick >> channelName;	
	if (channelName.empty() || targetNick.empty()) {
		std::string str = ERR_NEEDMOREPARAMS(handleClient->getUserName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	// Check if the first parameter is channel
	if (channelName[0] != '#') {
		const std::string str = ERR_INVERTPARAM(cmd.command, channelName);
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	//---------------------------------------------------
	// channel exist?
	std::vector<Channel>::iterator channelIt = channels.begin();
	while (channelIt != channels.end()) {
		if (channelIt->getName() == channelName) {
			break;
		}
		channelIt++;
	}
	if (channelIt == channels.end()) {
		std::string str = ERR_NOSUCHCHANNEL(this->name, handleClient->getUserName(), channelIt->getName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	// is inviter on channel? -ERR_NOTONCHANNEL
	std::vector<Client *>::iterator inviterIt = channelIt->getJoinedClients().begin();
	while (inviterIt != channelIt->getJoinedClients().end()) {
		if ((*inviterIt)->getNickName() == handleClient->getNickName()) {
			break;
		}
		inviterIt++;
	}
	if (inviterIt == channelIt->getJoinedClients().end()) {
		std::string str = ERR_NOTONCHANNEL(handleClient->getNickName(), channelIt->getName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	//------------------------------------------------
	// target already in channel -ERR_USERONCHANNEL
	std::vector<Client *>::iterator targetIt = channelIt->getJoinedClients().begin();
	while (targetIt != channelIt->getJoinedClients().end()) {
		if ((*targetIt)->getNickName() == targetNick) {
			std::string str = ERR_USERONCHANNEL(handleClient->getUserName(), (*targetIt)->getNickName(), channelIt->getName());
			send(handleClient->getSocket(), str.c_str(), str.length(), 0);
			return;
		}
		targetIt++;
	}
	// target exist in Server?
	std::vector<Client>::iterator targetExistIt = this->getClients().begin();
	while (targetExistIt != this->getClients().end()) {
		if (targetExistIt->getNickName() == targetNick) {
			break;
		}
		targetExistIt++;
	}
	if (targetExistIt == this->getClients().end()) {
		std::string str = ERR_NOTEXIST(targetExistIt->getNickName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	std::cout << "Debug 1" << std::endl;
	channelIt->getJoinedClients().push_back(&(*targetExistIt));
	std::string str = RPL_INVITING(handleClient->getUserName(), handleClient->getNickName(), targetExistIt->getNickName(), channelIt->getName());
	send(handleClient->getSocket(), str.c_str(), str.length(), 0);
}

// + ADD COMMENT
void Server::handleKick(Client* handleClient, const Command &cmd) {
	std::istringstream iss(cmd.parameter); // cmd.parameter has two words
	std::string channelName, targetNick;
	iss >> channelName >> targetNick;
	if (channelName.empty() || targetNick.empty()) {
		const std::string str = ERR_NEEDMOREPARAMS(handleClient->getUserName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	// Check if the first parameter is channel
	if (channelName[0] != '#') {
		const std::string str = ERR_INVERTPARAM(cmd.command, channelName);
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	// Search for the channel
	std::vector<Channel>::iterator channelIt = channels.begin();
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channelIt->getName() == channelName) {
			break;
		}
		channelIt++;
	}
	if (channelIt == channels.end()) {
		std::string str = ERR_NOSUCHCHANNEL(this->name, handleClient->getUserName(), channelIt->getName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	//--------------------------------------------------------------------
	// Search for the target client in the channel
	std::vector<Client *>::iterator targetIt = channelIt->getJoinedClients().begin();
	while (targetIt != channelIt->getJoinedClients().end()) {
		if ((*targetIt)->getNickName() == targetNick) {
			break;
		}
		targetIt++;
	}
	if (targetIt == channelIt->getJoinedClients().end()) {
		const std::string str = ERR_USERNOTINCHANNEL(handleClient->getUserName(), targetNick, channelIt->getName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	// Remove the client from the channel
	channelIt->removeClientFromList(targetIt);
	// Remove Client from operator list
	std::vector<Client *>::iterator operatorIt = channelIt->getOperators().begin();
	while (operatorIt != channelIt->getOperators().end()) {
		if ((*operatorIt)->getNickName() == targetNick) {
			operatorIt = channelIt->getOperators().erase(operatorIt);
		} else {
			++operatorIt;
		}
	}
	const std::string str = RPL_KICK(handleClient->getNickName(),  handleClient->getUserName(), channelIt->getName(), (*targetIt)->getUserName(), cmd.message);
	send(handleClient->getSocket(), str.c_str(), str.length(), 0);
}

bool Server::findChannelByName(const std::string& str) {
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (it->getName() == str) {
			return true;
		}
	}
	return false;
}


void Server::handleQuit(Client *handleClient, const Command &cmd) {
	std::vector<Client>::iterator clientIt = findObject(handleClient->getSocket(), clients);
	std::vector<pollfd>::iterator pollIt = poll_fds.begin();
	while (pollIt != poll_fds.end()) {
		if (pollIt->fd == handleClient->getSocket()) {
			break;
		}
		pollIt++;
	}
	if (pollIt == poll_fds.end()) {
		std::cerr << "Debug: Poll not found for QUIT command" << std::endl;
		return;
	}
	std::string str = RPL_QUIT(clientIt->getNickName(), clientIt->getUserName(), cmd.message);
	send(handleClient->getSocket(), str.c_str(), str.length(), 0);
	deleteClient(clientIt, pollIt);
}


void Server::handlePing(Client *handleClient, const Command &cmd) {
	if (cmd.parameter.empty()) {
		std::string str = ERR_NEEDMOREPARAMS(handleClient->getUserName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return ;
	}
	else {
		std::string line = "PONG " + cmd.parameter + "\r\n";
		send(handleClient->getSocket(), line.c_str(), line.length(), 0);
		return ;
	}

}


void Server::handlePong(Client *handleClient) {
	(void)handleClient;
	return ;
}