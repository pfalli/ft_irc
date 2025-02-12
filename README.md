# socket.cpp

run in two different terminals:

`c++ socket.cpp`  and  `nc 127.0.0.1 6697`

# IRC

`Internet Relay Chat` or IRC is a text-based communication protocol for live internet chat and communication. .
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.

To create an IRC client, you'll need to understand the protocol and implement its functionality, such as connecting to an IRC server, sending/receiving messages, handling channels, and interacting with the server's commands.

# Structure:

**Client-Server Communication**

    Clients connect to the server via TCP.
    Commands like JOIN, PART, PRIVMSG, and KICK are handled.
    Multiple users can join channels and communicate.

**Core Components**

    Server: Manages connections, messages, and channels.
    Client Handler: Handles incoming commands.
    Message Parser: Parses and executes IRC commands.
    Channel Management: Handles user permissions, topics, and bans.

**Protocol Flow**

    User connects (PASS, NICK, USER)
    User joins a channel (JOIN #channel)
    User sends messages (PRIVMSG or NOTICE)
    User leaves (PART, QUIT)
    Admin commands (KICK, BAN)

# Features
- **Client-Server Communication** using TCP sockets
- **Support for multiple users and channels**
- **Basic IRC commands:**
  - Authentication (`PASS`, `NICK`, `USER`)
  - Messaging (`PRIVMSG`, `NOTICE`)
  - Channel management (`JOIN`, `PART`, `KICK`, `BAN`, `MODE`)
  - Connection management (`QUIT`, `PING`, `PONG`)
- **User and channel permissions**
- **Handling multiple clients simultaneously**




`https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/`