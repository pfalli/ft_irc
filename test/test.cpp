#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

#define SERVER_NAME "irc.libera.chat"

#define BUF_SIZE 512

// Create a socket and connect to the server
int createConnection(const std::string& server, const std::string& port) {
    int sock;
    struct sockaddr_in serverAddr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed!" << std::endl;
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::stoi(port));
    serverAddr.sin_addr.s_addr = inet_addr(server.c_str());

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed!" << std::endl;
        exit(1);
    }

    return sock;
}

// Send a message to the IRC server
void sendMessage(int sock, const std::string& message) {
    if (send(sock, message.c_str(), message.length(), 0) < 0) {
        std::cerr << "Error sending message!" << std::endl;
        exit(1);
    }
}

// Receive a message from the IRC server
std::string receiveMessage(int sock) {
    char buffer[BUF_SIZE];
    int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0) {
        std::cerr << "Error receiving message!" << std::endl;
        exit(1);
    }
    return std::string(buffer, bytesReceived);
}

int main() {
    std::string server = SERVER_NAME;
    std::string port = "6697";

    int sock = createConnection(server, port);

    // Send NICK and USER commands
    sendMessage(sock, "NICK MyNickname\r\n");
    sendMessage(sock, "USER MyNickname 0 * :My real name\r\n");

    // Receive server messages
    while (true) {
        std::string serverMessage = receiveMessage(sock);
        std::cout << "Server says: " << serverMessage << std::endl;

        // Respond to PING messages to keep the connection alive
        if (serverMessage.find("PING") != std::string::npos) {
            sendMessage(sock, "PONG :pingis\n");
        }

        // Send a message to a channel or user
        if (serverMessage.find("Welcome") != std::string::npos) {
            sendMessage(sock, "JOIN #testchannel\r\n");
            sendMessage(sock, "PRIVMSG #testchannel :Hello, IRC!\r\n");
        }
    }

    close(sock);
    return 0;
}
