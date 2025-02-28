#include "Server.hpp"
#include "utils.hpp"
#include "Channel.hpp"
#include "NumericMessages.hpp"

void Server::handleCommand(const Command &cmd, Client &client) {
	if (cmd.command == "PASS")
		_register(client, cmd, PASSWORD);
	else if (cmd.command == "USER")
		_register(client, cmd, USERNAME);
	else if (cmd.command == "NICK")
		_register(client, cmd, NICKNAME);
	if (!client.getRegistered())
		return ;
	if (cmd.command == "JOIN") {
		join(this, &client, cmd.parameter);
	} else if (cmd.command == "MODE") {
		std::cout << "Handling MODE: "<< cmd.parameter + cmd.message<< std::endl;
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
    else if (cmd.command == "INVITE") {
		handleInvite(&client, cmd);
	}
	else if (cmd.command == "INFO") {
		printInfo(&client, cmd);
	}
	else {
		std::cout << "Command not found: " << cmd.command << std::endl;
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
	std::vector<Client>::iterator listIt = channelIt->getJoinedClients().begin();
	std::string clientList = "Clients in channel " + cmd.parameter + ":\n";
	while (listIt != channelIt->getJoinedClients().end()) {
		clientList += " => " + listIt->getNickName() + "\n";
		listIt++;
	}
	send(handleClient->getSocket(), clientList.c_str(), clientList.length(), 0);

}

// INVITE <nickname> <channel>
// 2 Issues: 1. when try to invite someone not inside the channel
void Server::handleInvite(Client* handleClient, const Command &cmd) {
	std::istringstream iss(cmd.parameter);
	std::string channelName, targetNick;
	iss >> targetNick >> channelName;	
	if (channelName.empty() || targetNick.empty()) {
		std::string str = ERR_NEEDMOREPARAMS(handleClient->getUserName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	// channel exist? -ERR_NOSUCHCHANNEL
	std::vector<Channel>::iterator channelIt = channels.begin();
	while (channelIt != channels.end()) {
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
	// is inviter on channel? -ERR_NOTONCHANNEL
	std::vector<Client>::iterator inviterIt = channelIt->getJoinedClients().begin();
	while (inviterIt != channelIt->getJoinedClients().end()) {
		if (inviterIt->getNickName() == handleClient->getNickName()) {
			break;
		}
		inviterIt++;
	}
	if (inviterIt == channelIt->getJoinedClients().end()) {
        std::string str = ERR_NOTONCHANNEL(handleClient->getNickName(), channelIt->getName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	// target already in channel -ERR_USERONCHANNEL
	std::vector<Client>::iterator targetIt = channelIt->getJoinedClients().begin();
	while (targetIt != channelIt->getJoinedClients().end()) {
		if (targetIt->getNickName() == targetNick) {
			std::string str = ERR_USERONCHANNEL(handleClient->getUserName(), targetIt->getNickName(), channelIt->getName());
			send(handleClient->getSocket(), str.c_str(), str.length(), 0);
			return;
		}
		targetIt++;
	}
	// target exist?
	std::vector<Client>::iterator targetIt = channelIt->getJoinedClients().begin();
	while (targetIt != channelIt->getJoinedClients().end()) {
		if (targetIt->getNickName() == targetNick) {
			std::string str = ERR_USERONCHANNEL(handleClient->getUserName(), targetIt->getNickName(), channelIt->getName());
			send(handleClient->getSocket(), str.c_str(), str.length(), 0);
			return;
		}
		targetIt++;
	}
	channelIt->getJoinedClients().push_back(*targetIt);
	std::string str = RPL_INVITING(handleClient->getUserName(), handleClient->getNickName(), targetIt->getNickName(), channelIt->getName());
	send(handleClient->getSocket(), str.c_str(), str.length(), 0);


}

// Issue: 1. when channel and name are inverted!!!
// ***it doesn't kick multiple clients*** only member of channel can kick clients!!! + ADD COMMENT
void Server::handleKick(Client* handleClient, const Command &cmd) {
	std::istringstream iss(cmd.parameter); // cmd.parameter has two words
	std::string channelName, targetNick;
	iss >> channelName >> targetNick;	
	if (channelName.empty() || targetNick.empty()) {
		const std::string str = ERR_NEEDMOREPARAMS(handleClient->getUserName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	// Search for the channel
	std::vector<Channel>::iterator channelIt = channels.begin();
	while (channelIt != channels.end()) {
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
	// Search for the target client in the channel
	std::vector<Client>::iterator targetIt = channelIt->getJoinedClients().begin();
	while (targetIt != channelIt->getJoinedClients().end()) {
		if (targetIt->getNickName() == targetNick) {
			break;
		}
		targetIt++;
	}
	if (targetIt == channelIt->getJoinedClients().end()) {
        std::string str = ERR_USERNOTINCHANNEL(handleClient->getUserName(), targetIt->getNickName(), channelIt->getName());
		send(handleClient->getSocket(), str.c_str(), str.length(), 0);
		return;
	}
	// Remove the client from the channel
	channelIt->removeClientFromList(targetIt);
	const std::string str = RPL_KICK(handleClient->getNickName(),  handleClient->getUserName(), channelIt->getName(), targetIt->getUserName());
	send(handleClient->getSocket(), str.c_str(), str.length(), 0);
	// std::cout << "Client " << targetNick << " kicked from channel " << channelName << std::endl;
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