#include "Server.hpp"
#include "utils.hpp"
#include "Channel.hpp"
#include "NumericMessages.hpp"

void Server::handleCommand(const Command &cmd, Client &client) {
	if (cmd.command == "HELP")
		giveHelp(client);
	else if (cmd.command == "CAP")
		checkCaseHex(cmd, client);
	else if (cmd.command == "PASS")
		_register(client, cmd, PASSWORD);
	else if (cmd.command == "USER")
		_register(client, cmd, USERNAME);
	else if (cmd.command == "NICK")
		_register(client, cmd, NICKNAME);
	if (!client.getRegistered() || !client.getPW())
		return ;
	if (cmd.command == "JOIN") {
		join(this, &client, cmd);
	}
	else if (cmd.command == "CLIENTS") {
		numClients(client);
	}
	else if (cmd.command == "MODE") {
		mode(this, cmd, &client);
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
		topic(this, cmd, &client);
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

void Server::numClients(Client &requestingClient) {
	std::string clientList = "Clients connected to the server:\n";
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		clientList += " => " + it->getNickName() + "\n";
	}
	send(requestingClient.getSocket(), clientList.c_str(), clientList.length(), 0);
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
	PRIVMSG <nickname> or #<channel name> : <message> to send a private message to a user or channel\n\
	TOPIC #<channel name> : <topic> to set the topic of a channel\n\
	QUIT <reason> to leave our server\n";
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
			std::string str = ERR_NOSUCHCHANNEL(handleClient->getUserName(), target);
			send(handleClient->getSocket(), str.c_str(), str.length(), 0);
			return;
		}
		if (channel->isUserInChannel(handleClient->getNickName()) == NULL) {
			std::string str = ERR_NOTONCHANNEL(handleClient->getNickName(), target);
			send(handleClient->getSocket(), str.c_str(), str.length(), 0);
			return;
		}
		std::string str = RPL_NOTICE(handleClient->getNickName(), handleClient->getUserName(), target, cmd.message);
		const char *msg = str.c_str();
		sendToChannel(*channel, msg, handleClient->getSocket()); //might need to send notice to handleClient as well
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
	if (cmd.parameter.empty())
		return ;
	std::vector<Channel>::iterator channelIt = channels.begin();
	while (channelIt != channels.end()) {
		if (channelIt->getName() == cmd.parameter) {
			break;
		}
		channelIt++;
	}
	if (channelIt == channels.end()) {
		std::string msg = ERR_NOSUCHCHANNEL(handleClient->getNickName(), channelIt->getName());
		send(handleClient->getSocket(), msg.c_str(), strlen(msg.c_str()), 0);
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
	std::string targetNick, channelName;
	iss >> targetNick >> channelName;

	if (channelName.empty() || targetNick.empty()) {
		std::string str = ERR_NEEDMOREPARAMS(handleClient->getUserName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	if (targetNick.find(" ") != std::string::npos) {
		std::string str = ERR_TOOMANYPARAMS(handleClient->getUserName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}

	if (channelName[0] != '#') {
		std::string str = ERR_NOSUCHCHANNEL(handleClient->getUserName(), channelName);
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}

	std::vector<Channel>::iterator channelIt = channels.begin();
	while (channelIt != channels.end()) {
		if (channelIt->getName() == channelName) {
			break;
		}
		channelIt++;
	}

	if (channelIt == channels.end()) {
		std::string str = ERR_NOSUCHCHANNEL(handleClient->getUserName(), channelName);
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}

	if (!isOperator(*channelIt, handleClient))
	{
		// std::string error = "Error: you need to be operator to do this.\n";
		// send(handleClient->getSocket(), error.c_str(), error.length(), 0);
		sendMsg(handleClient, ERR_CHANOPRIVSNEEDED(handleClient->getNickName(), channelIt->getName())); // FIX it later
		return ;
	}

	std::vector<Client *>::iterator inviterIt = channelIt->getJoinedClients().begin();
	while (inviterIt != channelIt->getJoinedClients().end()) {
		if ((*inviterIt)->getNickName() == handleClient->getNickName()) {
			break;
		}
		inviterIt++;
	}

	if (inviterIt == channelIt->getJoinedClients().end()) {
		std::string str = ERR_NOTONCHANNEL(handleClient->getNickName(), channelName);
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}

	std::vector<Client *>::iterator targetIt = channelIt->getJoinedClients().begin();
	while (targetIt != channelIt->getJoinedClients().end()) {
		if ((*targetIt)->getNickName() == targetNick) {
			std::string str = ERR_USERONCHANNEL(handleClient->getUserName(), targetNick, channelName);
			send(handleClient->getSocket(), str.c_str(), str.length(), 0);
			return;
		}
		targetIt++;
	}

	// std::vector<Client>::iterator targetExistIt = this->getClients().begin();
	
	size_t i;
	for (i = 0; i < this->getClients().size(); i++)
	{
		if (this->getClients()[i].getNickName() == targetNick) {
			break;
		}
	}
	
	// for (size_t i = 0; i < this->getClients().size(); i++)
	// {
	// 	if (targetExistIt == this->getClients().end())
	// 		break ;
	// 	std::cout << std::endl << "test print " << i << std::endl;
	// 	if (targetExistIt->getNickName() == targetNick) {
	// 	   break;
	//    }
	//    targetExistIt++;
	// }
	
	// while (targetExistIt != this->getClients().end()) {
	// 	std::cout << std::endl << "test print " << targetExistIt->getSocket() << std::endl;
 	// 	if (targetExistIt->getNickName() == targetNick) {
	// 		break;
	// 	}
	// 	targetExistIt++;
	// }

	if (i == this->getClients().size()){
	//if (targetExistIt == this->getClients().end()) {
		std::string str = ERR_NOSUCHNICK(handleClient->getNickName(), targetNick);
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}

	std::string confirmMsg = ":" + this->name + " 341 " + handleClient->getNickName() + " " + targetNick + " " + channelName + "\r\n";
	send(handleClient->getSocket(), confirmMsg.c_str(), confirmMsg.length(), 0);
	std::string inviteMsg = ":" + handleClient->getNickName() + "!" + handleClient->getUserName() + "@" + this->name + " INVITE " + targetNick + " :" + channelName + "\r\n";
	send(this->getClients()[i].getSocket(), inviteMsg.c_str(), inviteMsg.length(), 0);

	join(this, &this->getClients()[i], cmd);
}

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
		std::string str = ERR_NOSUCHCHANNEL(handleClient->getUserName(), channelIt->getName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	if (!isOperator(*channelIt, handleClient))
	{
		// std::string error = "Error: you need to be operator to do this.\n";
		// send(handleClient->getSocket(), error.c_str(), error.length(), 0);
		sendMsg(handleClient, ERR_CHANOPRIVSNEEDED(handleClient->getNickName(), channelIt->getName())); // FIX it later
		return ;
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
	const std::string str = RPL_KICK(handleClient->getNickName(),  handleClient->getUserName(), channelIt->getName(), (*targetIt)->getNickName(), cmd.message);
	const std::string msg = RPL_KICK((*targetIt)->getNickName(),  (*targetIt)->getUserName(), channelIt->getName(), (*targetIt)->getNickName(), cmd.message);
	send(handleClient->getSocket(), str.c_str(), str.length(), 0);
	send((*targetIt)->getSocket(), msg.c_str(), msg.length(), 0);
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
	// send to all people inside all channels
	std::string str = RPL_QUIT(clientIt->getNickName(), cmd.message);
	const char *msg = str.c_str();
	std::vector<Channel>::iterator channelIt = channels.begin();
	for (size_t i = 0; i < channels.size(); i++) {
		sendToChannel(*channelIt, msg, handleClient->getSocket());
	}
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