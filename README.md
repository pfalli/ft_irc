# test

run in two different terminals:

`c++ socket.cpp`  and  `nc 127.0.0.1 6697`

# IRC

`Internet Relay Chat` or IRC is a text-based communication protocol for live internet chat and communication. .
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.

To create an IRC client, you'll need to understand the protocol and implement its functionality, such as connecting to an IRC server, sending/receiving messages, handling channels, and interacting with the server's commands.


```
                       +----------------------+
                       |   IRC Network        |
                       +----------------------+
                                 |
     ------------------------------------------------
     |                                              |
+-------------------+                        +-------------------+
|    IRC Server 1   |------------------------|    IRC Server 2   |
+-------------------+                        +-------------------+
         |                                            |
         |                                            |
+-------------------+                        +-------------------+
|   IRC Channel A   |                        |   IRC Channel B   |
+-------------------+                        +-------------------+
      |       |                                    |       |
      |       |                                    |       |
+------------+------------+              +------------+------------+
|   IRC Client A   |   IRC Client B   |  |   IRC Client C   |   IRC Client D   |
+------------------+------------------+  +------------------+------------------+
```

# Structure:

1. Connect to the Server

The client needs to establish a TCP/IP connection to the IRC server using the server's IP address and port (usually port 6667 for non-SSL connections).
2. Send the NICK and USER Commands

The IRC protocol requires clients to identify themselves with a nickname (NICK command) and user information (USER command). This is the first step after the connection is made.
3. Join a Channel

Once connected, you can join a channel (like a chat room) using the JOIN command. This is how users communicate in IRC.
4. Sending Messages

The client can send messages to a channel or to individual users using the PRIVMSG command. Messages can be sent to specific channels or users.
5. Receiving Messages

The client needs to listen for incoming messages from the server. The server will send updates, including messages from other users, server notices, and information about the status of channels.
6. Handling Commands

The client needs to process and handle different IRC commands such as PING, PONG, PART, QUIT, and more. This is important for maintaining the connection and responding to server requests.
7. Disconnecting from the Server

To exit the IRC session, the client sends a QUIT message, which tells the server that the client is leaving.


`https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/`